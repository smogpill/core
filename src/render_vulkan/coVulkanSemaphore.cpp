// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanSemaphore.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanSemaphore::coVulkanSemaphore()
	: semaphore_vk(VK_NULL_HANDLE)
{

}

coVulkanSemaphore::~coVulkanSemaphore()
{
	if (semaphore_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		vkDestroySemaphore(device_vk, semaphore_vk, nullptr);
	}
}

const VkDevice& coVulkanSemaphore::GetVkDevice() const
{
	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice;
}

coResult coVulkanSemaphore::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	//const InitConfig& config = static_cast<const InitConfig&>(_config);

	VkSemaphoreCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);

	coASSERT(semaphore_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateSemaphore(device_vk, &createInfo, nullptr, &semaphore_vk), "Failed to create Vulkan semaphore.");
	return true;
}
