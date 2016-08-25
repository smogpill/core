// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanCommandPool.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coVulkanCommandPool::coVulkanCommandPool()
	: commandPool_vk(VK_NULL_HANDLE)
{

}

coVulkanCommandPool::InitConfig::InitConfig()
	: queueFamilyIndex(-1)
{

}

coVulkanCommandPool::~coVulkanCommandPool()
{
	if (commandPool_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		vkDestroyCommandPool(device_vk, commandPool_vk, nullptr);
	}
}

coResult coVulkanCommandPool::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	coNumericConvert(poolInfo.queueFamilyIndex, config.queueFamilyIndex);
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	const VkDevice& device_vk = GetVkDevice();
	coASSERT(device_vk != VK_NULL_HANDLE);
	coASSERT(commandPool_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateCommandPool(device_vk, &poolInfo, nullptr, &commandPool_vk), "Failed to create a Vulkan command pool.");;

	return true;
}

const VkDevice& coVulkanCommandPool::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}
