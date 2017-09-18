// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanShader.h"
#include "render_vulkan/coVulkanPipelineLayout.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanDescriptorSet.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coType_f.h"
#include "lang/reflect/coField.h"
#include "math/vector/coFloatx2.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx4.h"
#include "math/matrix/coMat4_f.h"
#include "math/quaternion/coQuat_f.h"
#include "math/transform/coTransform.h"

// Hack
struct ModelConstants
{
	coMat4 modelViewProj;
};

coVulkanPipeline::coVulkanPipeline()
	: pipeline_vk(VK_NULL_HANDLE)
	, vulkanDescriptorSet(nullptr)
	, vulkanBuffer(nullptr)
	, vulkanPipelineLayout(nullptr)
{
	vulkanBuffer = new coVulkanBuffer();
	vulkanDescriptorSet = new coVulkanDescriptorSet();
}

coVulkanPipeline::~coVulkanPipeline()
{
	if (pipeline_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		vkDestroyPipeline(device_vk, pipeline_vk, nullptr);
	}

	delete vulkanDescriptorSet;
	delete vulkanBuffer;
}

coResult coVulkanPipeline::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	{
		coHACK("Hard coded vulkan buffer");
	
		coVulkanBuffer::InitConfig c;
		c.debugName = "Hard coded vulkan buffer";
		c.device = config.device;
		c.size8 = sizeof(ModelConstants);
		c.type = coVulkanBuffer::dynamic;
		c.usage = coVulkanBuffer::Usage::uniform;
		coTRY(vulkanBuffer->Init(c), nullptr);

		coTRY(UpdateConstants(coVec3(0.0f)), nullptr);
	}

	{
		coHACK("Hard coded descriptor set.");
		coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(config.device);
		coVulkanDescriptorSet::InitConfig c;
		c.device = config.device;
		c.debugName = "Hard coded descriptor set";
		c.vulkanBuffer = vulkanBuffer;
		c.vulkanDescriptorPool = vulkanLogicalDevice->GetVulkanDescriptorPool();
		coTRY(vulkanDescriptorSet->Init(c), nullptr);
	}

	// Shader stages
	coDynamicArray<VkPipelineShaderStageCreateInfo> shaderInfos;
	coTRY(InitShaderStages(shaderInfos, config), "Failed to init shader stages.");

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	coDynamicArray<VkVertexInputBindingDescription> vertexBindings;
	coDynamicArray<VkVertexInputAttributeDescription> vertexAttributes;
	coTRY(InitVertexInput(vertexInputInfo, vertexBindings, vertexAttributes, config), "Failed to init the vertex inputs.");

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	coTRY(InitInputAssembly(inputAssemblyInfo, config), "Failed to init the input assembly.");

	// Viewport
	VkPipelineViewportStateCreateInfo viewportInfo;
	coDynamicArray<VkViewport> viewports;
	coDynamicArray<VkRect2D> scissors;
	coTRY(InitViewport(viewportInfo, viewports, scissors, config), "Failed to init the viewport.");

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizerInfo;
	coTRY(InitRasterizer(rasterizerInfo, config), "Failed to init the rasterizer.");

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisamplingInfo;
	coTRY(InitMultisampling(multisamplingInfo, config), "Failed to init multisampling.");

	// Color blending
	VkPipelineColorBlendStateCreateInfo colorBlendingInfo;
	coDynamicArray<VkPipelineColorBlendAttachmentState> attachments;
	coTRY(InitColorBlending(colorBlendingInfo, attachments, config), "Failed to init color blending.");

	// Pipeline Layout
	const coVulkanPipelineLayout* vulkanLayout = static_cast<const coVulkanPipelineLayout*>(config.layout);
	coTRY(vulkanLayout, nullptr);
	const VkPipelineLayout& layout_vk = vulkanLayout->GetVkPipelineLayout();
	vulkanPipelineLayout = vulkanLayout;

	// Render pass
	const coVulkanPass* vulkanPass = static_cast<const coVulkanPass*>(config.renderPass);
	coTRY(vulkanPass, nullptr);
	const VkRenderPass& renderPass_vk = vulkanPass->GetVkRenderPass();

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderInfos.count;
	pipelineInfo.pStages = shaderInfos.data;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisamplingInfo;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlendingInfo;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = layout_vk;
	pipelineInfo.renderPass = renderPass_vk;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	const VkDevice& device_vk = GetVkDevice();
	coVULKAN_TRY(vkCreateGraphicsPipelines(device_vk, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_vk), "Failed to create the vulkan pipeline.");

	return true;
}

coResult coVulkanPipeline::UpdateConstants(const coVec3& _rotation)
{
	coMat4 model;
	{
		coTransform t;
		t.rotation = coRotation(_rotation);
		coSetWithoutScale(model, t);
	}

	coMat4 view;
	{
		coTransform t;
		t.translation = coVec3(0, 0, -5);
		coSetWithoutScale(view, t);
	}

	coMat4 proj;
	{
		coSetPerspective(proj, coFloat_halfPi, 16.f / 9.f, 0.05f, 1000.0f);
	}

	ModelConstants constants;
	constants.modelViewProj = proj * view * model;

	void* data = nullptr;
	coTRY(vulkanBuffer->Map(data), nullptr);
	*reinterpret_cast<ModelConstants*>(data) = constants;
	vulkanBuffer->Unmap();
	return true;
}

coResult coVulkanPipeline::InitShaderStages(coDynamicArray<VkPipelineShaderStageCreateInfo>& _out, const InitConfig& _config)
{
	coReserve(_out, _config.shaders.count);
	for (const coShader* shader : _config.shaders)
	{
		coTRY(shader, nullptr);
		const coVulkanShader* vulkanShader = static_cast<const coVulkanShader*>(shader);
		VkPipelineShaderStageCreateInfo& shaderInfo = coPushBack(_out, VkPipelineShaderStageCreateInfo{});
		shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfo.stage = vulkanShader->GetVkShaderStageFlagBits();
		shaderInfo.module = vulkanShader->GetVkShaderModule();
		shaderInfo.pName = "main";
		shaderInfo.pSpecializationInfo = nullptr;
		shaderInfo.flags = 0;
	}
	return true;
}

coResult _coGetAttributeFormat(VkFormat& _out, const coType& _type)
{
	coTODO("Write a simpler and faster implementation");
	static const coUint32 coFloatHash = coGetType<coFloat>()->nameHash;
	static const coUint32 coFloatx2Hash = coGetType<coFloatx2>()->nameHash;
	static const coUint32 coFloatx3Hash = coGetType<coFloatx3>()->nameHash;
	static const coUint32 coFloatx4Hash = coGetType<coFloatx4>()->nameHash;
	if (_type.nameHash == coFloatHash)
	{
		_out = VK_FORMAT_R32_SFLOAT;
		return true;
	}
	else if (_type.nameHash == coFloatx2Hash)
	{
		_out = VK_FORMAT_R32G32_SFLOAT;
		return true;
	}
	else if (_type.nameHash == coFloatx3Hash)
	{
		_out = VK_FORMAT_R32G32B32_SFLOAT;
		return true;
	}
	else if (_type.nameHash == coFloatx4Hash)
	{
		_out = VK_FORMAT_R32G32B32A32_SFLOAT;
		return true;
	}
	return false;
}

coResult coVulkanPipeline::InitVertexInput(VkPipelineVertexInputStateCreateInfo& _out, coDynamicArray<VkVertexInputBindingDescription>& _outBindings, coDynamicArray<VkVertexInputAttributeDescription>& _outAttributes, const InitConfig& _config)
{
	coASSERT(_outBindings.count == 0);
	coASSERT(_outAttributes.count == 0);
	coReserve(_outBindings, _config.vertexChannels.count);
	coUint32 nbAttributes = 0;
	for (const coType* type : _config.vertexChannels)
	{
		coTRY(type, nullptr);
		nbAttributes += type->fields.count;
	}
	coReserve(_outAttributes, nbAttributes);

	coUint32 location = 0;
	for (coUint32 i = 0; i < _config.vertexChannels.count; ++i)
	{
		const coType* type = _config.vertexChannels[i];
		VkVertexInputBindingDescription& binding_vk = coPushBack(_outBindings, VkVertexInputBindingDescription{});
		binding_vk.binding = i;
		binding_vk.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		binding_vk.stride = type->size8;
		for (const coField* field : type->fields)
		{
			coTRY(field, nullptr);
			const coType* fieldType = field->type;
			coTRY(fieldType, nullptr);
			VkVertexInputAttributeDescription& attribute_vk = coPushBack(_outAttributes, VkVertexInputAttributeDescription{});
			attribute_vk.binding = i;
			attribute_vk.location = location;
			attribute_vk.offset = field->offset8;
			coTRY(_coGetAttributeFormat(attribute_vk.format, *fieldType), "Field type unsupported for conversion to Vulkan: "<<fieldType->name);
			++location;
		}
	}

	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_out.vertexBindingDescriptionCount = _outBindings.count;
	_out.pVertexBindingDescriptions = _outBindings.data;
	_out.vertexAttributeDescriptionCount = _outAttributes.count;
	_out.pVertexAttributeDescriptions = _outAttributes.data;
	return true;
}

coResult coVulkanPipeline::InitInputAssembly(VkPipelineInputAssemblyStateCreateInfo& _out, const InitConfig& /*_config*/)
{
	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	_out.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	_out.primitiveRestartEnable = VK_FALSE;
	return true;
}

coResult coVulkanPipeline::InitViewport(VkPipelineViewportStateCreateInfo& _out, coDynamicArray<VkViewport>& _outViewports, coDynamicArray<VkRect2D>& _outScissors, const InitConfig& _config)
{
	// Viewports
	coReserve(_outViewports, _config.viewports.count);
	for (const coFloatx2Rect& viewport : _config.viewports)
	{
		VkViewport& viewport_vk = coPushBack(_outViewports, VkViewport{});
		viewport_vk.x = viewport.pos.x;
		viewport_vk.y = viewport.pos.y;
		viewport_vk.width = viewport.size.x;
		viewport_vk.height = viewport.size.y;
		viewport_vk.minDepth = 0.0f;
		viewport_vk.maxDepth = 1.0f;
	}

	// Scissors
	coReserve(_outScissors, _config.scissors.count);
	for (const coInt32x2Rect& scissor : _config.scissors)
	{
		VkRect2D& scissor_vk = coPushBack(_outScissors, VkRect2D{});
		scissor_vk.offset.x = scissor.pos.x;
		scissor_vk.offset.y = scissor.pos.y;
		scissor_vk.extent.width = scissor.size.x;
		scissor_vk.extent.height = scissor.size.y;
	}

	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	_out.viewportCount = _outViewports.count;
	_out.pViewports = _outViewports.data;
	_out.scissorCount = _outScissors.count;
	_out.pScissors = _outScissors.data;
	return true;
}

coResult coVulkanPipeline::InitRasterizer(VkPipelineRasterizationStateCreateInfo& _out, const InitConfig& /*_config*/)
{
	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	_out.polygonMode = VK_POLYGON_MODE_FILL;
	_out.lineWidth = 1.0f;
	_out.cullMode = VK_CULL_MODE_BACK_BIT;
	_out.frontFace = VK_FRONT_FACE_CLOCKWISE;
	return true;
}

coResult coVulkanPipeline::InitMultisampling(VkPipelineMultisampleStateCreateInfo& _out, const InitConfig& /*_config*/)
{
	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	_out.sampleShadingEnable = VK_FALSE;
	_out.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	_out.minSampleShading = 1.0f;
	_out.pSampleMask = nullptr;
	_out.alphaToCoverageEnable = VK_FALSE;
	_out.alphaToOneEnable = VK_FALSE;
	return true;
}

coResult coVulkanPipeline::InitColorBlending(VkPipelineColorBlendStateCreateInfo& _out, coDynamicArray<VkPipelineColorBlendAttachmentState>& _outAttachments, const InitConfig& /*_config*/)
{
	coReserve(_outAttachments, 1);
	VkPipelineColorBlendAttachmentState& attachment = coPushBack(_outAttachments, VkPipelineColorBlendAttachmentState{});
	attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachment.blendEnable = VK_FALSE;
	attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachment.colorBlendOp = VK_BLEND_OP_ADD;
	attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	_out = {};
	_out.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	_out.logicOpEnable = VK_FALSE;
	_out.logicOp = VK_LOGIC_OP_COPY;
	_out.attachmentCount = _outAttachments.count;
	_out.pAttachments = _outAttachments.data;
	_out.blendConstants[0] = 0.0f;
	_out.blendConstants[1] = 0.0f;
	_out.blendConstants[2] = 0.0f;
	_out.blendConstants[3] = 0.0f;

	return true;
}

const VkDevice& coVulkanPipeline::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}
