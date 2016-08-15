// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanBuffer::coVulkanBuffer()
	: buffer_vk(VK_NULL_HANDLE)
{

}

coVulkanBuffer::~coVulkanBuffer()
{
	if (buffer_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		vkDestroyBuffer(device_vk, buffer_vk, nullptr);
	}
}

coResult coVulkanBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);

	VkBufferUsageFlags usage_vk = 0;
	if (config.usage & Usage::vertex)
		usage_vk |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	if (config.usage & Usage::index)
		usage_vk |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	if (config.usage & Usage::uniform)
		usage_vk |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.size = size8;
	createInfo.usage = usage_vk;
	createInfo.flags = 0;
	createInfo.sharingMode = config.shared ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;

	coASSERT(buffer_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateBuffer(device_vk, &createInfo, nullptr, &buffer_vk), "Failed to create buffer.");

	return true;
}

const VkDevice& coVulkanBuffer::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}