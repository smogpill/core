// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderPipeline.h"

class coRenderDevice;
class coVulkanDescriptorSet;
class coVulkanPipelineLayout;
class coVulkanBuffer;
class coVec3;

class coVulkanPipeline : public coRenderPipeline
{
	coDECLARE_SUPER(coRenderPipeline);
public:
	coVulkanPipeline();
	virtual ~coVulkanPipeline();

	const VkPipeline& GetVkPipeline() const { return pipeline_vk; }

	// Hack
	coVulkanDescriptorSet* GetVulkanDescriptorSet() const { return vulkanDescriptorSet; }
	const coVulkanPipelineLayout* GetVulkanPipelineLayout() const { return vulkanPipelineLayout; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;
	coResult InitShaderStages(coDynamicArray<VkPipelineShaderStageCreateInfo>& _out, const InitConfig& _config);
	coResult InitVertexInput(VkPipelineVertexInputStateCreateInfo& _out, 
		coDynamicArray<VkVertexInputBindingDescription>& _outBindings, coDynamicArray<VkVertexInputAttributeDescription>& _outAttributes, const InitConfig& _config);
	coResult InitInputAssembly(VkPipelineInputAssemblyStateCreateInfo& _out, const InitConfig& _config);
	coResult InitViewport(VkPipelineViewportStateCreateInfo& _out, coDynamicArray<VkViewport>& _outViewports, coDynamicArray<VkRect2D>& _outScissors, const InitConfig& _config);
	coResult InitRasterizer(VkPipelineRasterizationStateCreateInfo& _out, const InitConfig& _config);
	coResult InitMultisampling(VkPipelineMultisampleStateCreateInfo& _out, const InitConfig& _config);
	coResult InitColorBlending(VkPipelineColorBlendStateCreateInfo& _out, coDynamicArray<VkPipelineColorBlendAttachmentState>& _outAttachments, const InitConfig& _config);

	VkPipeline pipeline_vk;

	// hack 
	coResult UpdateConstants(const coVec3& _rotation);
	coVulkanBuffer* vulkanBuffer;
	coVulkanDescriptorSet* vulkanDescriptorSet;
	const coVulkanPipelineLayout* vulkanPipelineLayout;
};
