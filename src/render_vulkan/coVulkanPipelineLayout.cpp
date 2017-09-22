// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanPipelineLayout.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanDescriptorSetLayout.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"

// Hack
#include "math/matrix/coMat4.h"

coVulkanPipelineLayout::coVulkanPipelineLayout()
	: pipelineLayout_vk(VK_NULL_HANDLE)
	, vulkandescriptorSetLayout(nullptr)
{
	vulkandescriptorSetLayout = new coVulkanDescriptorSetLayout();
}

coVulkanPipelineLayout::~coVulkanPipelineLayout()
{
	if (pipelineLayout_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		vkDestroyPipelineLayout(device_vk, pipelineLayout_vk, nullptr);
	}
	delete vulkandescriptorSetLayout;
}

coResult coVulkanPipelineLayout::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	{
		coHACK("Hard coded vulkan descriptor set layout");
		coVulkanDescriptorSetLayout::InitConfig c;
		c.debugName = "Hacked descriptor set layout";
		c.device = config.device;
		coTRY(vulkandescriptorSetLayout->Init(c), nullptr);
	}

	coHACK("Hard coded constants.");
	VkPushConstantRange constantRange_vk{};
	{
		constantRange_vk.offset = 0;
		constantRange_vk.size = sizeof(coMat4);
		constantRange_vk.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	}

	coDynamicArray<VkDescriptorSetLayout> descriptorSetLayouts_vk{ vulkandescriptorSetLayout->GetVkDescriptorSetLayout() };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts_vk.count;
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts_vk.data;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &constantRange_vk;

	const VkDevice& device_vk = GetVkDevice();
	coTRY(pipelineLayout_vk == VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkCreatePipelineLayout(device_vk, &pipelineLayoutInfo, nullptr, &pipelineLayout_vk), "Failed to create the Vulkan pipeline layout.");
	return true;
}

const VkDevice& coVulkanPipelineLayout::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}
