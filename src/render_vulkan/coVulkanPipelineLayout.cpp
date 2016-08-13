// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanPipelineLayout.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"

coVulkanPipelineLayout::coVulkanPipelineLayout()
	: pipelineLayout_vk(VK_NULL_HANDLE)
{

}

coVulkanPipelineLayout::~coVulkanPipelineLayout()
{
	if (pipelineLayout_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		vkDestroyPipelineLayout(device_vk, pipelineLayout_vk, nullptr);
	}
}

coResult coVulkanPipelineLayout::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	//const InitConfig& config = static_cast<const InitConfig&>(_config);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = 0;

	const VkDevice& device_vk = GetVkDevice();
	coTRY(pipelineLayout_vk == VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkCreatePipelineLayout(device_vk, &pipelineLayoutInfo, nullptr, &pipelineLayout_vk), "Failed to create the Vulkan pipeline layout.");
	return true;
}

const VkDevice& coVulkanPipelineLayout::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(logicalDevice);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}
