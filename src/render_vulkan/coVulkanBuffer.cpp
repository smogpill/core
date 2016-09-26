// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanDeviceAllocator.h"
#include "render_vulkan/coVulkanDeviceAllocation.h"
#include "render_vulkan/coVulkanDeviceMemoryChunk.h"
#include "lang/result/coResult_f.h"

coVulkanBuffer::coVulkanBuffer()
	: buffer_vk(VK_NULL_HANDLE)
	, deviceAllocation(nullptr)
{

}

coVulkanBuffer::~coVulkanBuffer()
{
	delete deviceAllocation;
	deviceAllocation = nullptr;

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
	coVulkanLogicalDevice* vulkanDevice = static_cast<coVulkanLogicalDevice*>(device);
	coTRY(vulkanDevice, nullptr);
	const VkDevice& device_vk = vulkanDevice->GetVkDevice();
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
	coASSERT(buffer_vk != VK_NULL_HANDLE);

	coVulkanDeviceAllocator* allocator = vulkanDevice->GetVulkanDeviceAllocator();
	coTRY(allocator, nullptr);
	coTRY(!deviceAllocation, nullptr);
	VkMemoryPropertyFlags flags_vk = 0;
	switch (config.type)
	{
	case coRenderBuffer::default_: flags_vk = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; break;
	case coRenderBuffer::staging: flags_vk = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT; break;
	case coRenderBuffer::dynamic: flags_vk = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; break;
	default: coWARN_NOT_AVAILABLE();
	}
	VkDeviceSize size_vk = size8;
	coTRY(allocator->Allocate(deviceAllocation, flags_vk, size_vk), "Failed to allocate the buffer in the device memory");
	coVULKAN_TRY(vkBindBufferMemory(device_vk, buffer_vk, deviceAllocation->chunk->deviceMemory_vk, deviceAllocation->offset_vk), "Failed to bind the buffer to the device memory.");

	return true;
}

const VkDevice& coVulkanBuffer::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

coResult coVulkanBuffer::InitMemoryHack()
{
	VkMemoryAllocateInfo allocInfo{};
	// 	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	// 	allocInfo.allocationSize = 256 * 1024 * 1024;
	// 	allocInfo.memoryTypeIndex = ;
	// 
	// 	const VkDevice& device_vk = GetVkDevice();
	// 	VkDeviceMemory deviceMemory_vk;
	// 	coVULKAN_TRY(vkAllocateMemory(device_vk, &allocInfo, nullptr, &deviceMemory_vk), "Failed to allocate device memory.");
	return true;
}

coResult coVulkanBuffer::OnStart()
{
	coTRY(Super::OnStart(), nullptr);
// 	coTRY(buffer_vk != VK_NULL_HANDLE, nullptr);
// 	coVulkanLogicalDevice* vulkanDevice = static_cast<coVulkanLogicalDevice*>(device);
// 	coTRY(vulkanDevice, nullptr);
// 	coVulkanDeviceAllocator* allocator = vulkanDevice->GetVulkanDeviceAllocator();
// 	coTRY(allocator, nullptr);
// 
// 	coTRY(!deviceAllocation, nullptr);
// 	VkMemoryPropertyFlags flags_vk = 0;
// 	VkDeviceSize size_vk = ;
// 	coTRY(allocator->Allocate(deviceAllocation, flags_vk, size_vk), "Failed to allocate the buffer in the device memory");
// 	const VkDevice& device_vk = vulkanDevice->GetVkDevice();
// 	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
// 	coVULKAN_TRY(vkBindBufferMemory(device_vk, buffer_vk, deviceAllocation->chunk->deviceMemory_vk, deviceAllocation->offset_vk), "Failed to bind the buffer to the device memory." ;
	return true;
}

void coVulkanBuffer::OnStop()
{

	Super::OnStop();
}

coResult coVulkanBuffer::Map(void*& _data)
{
	coTRY(Super::Map(_data), nullptr);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk, nullptr);
	coTRY(deviceAllocation, nullptr);
	VkMemoryMapFlags flags_vk = 0;
	coVULKAN_TRY(vkMapMemory(device_vk, deviceAllocation->chunk->deviceMemory_vk, deviceAllocation->offset_vk, deviceAllocation->size_vk, flags_vk, &_data), "Failed to map data: "<<*this);
	return true;
}

void coVulkanBuffer::Unmap()
{
	const VkDevice& device_vk = GetVkDevice();
	coCHECK(device_vk, nullptr);
	coCHECK(deviceAllocation, nullptr);
	vkUnmapMemory(device_vk, deviceAllocation->chunk->deviceMemory_vk);
	Super::Unmap();
}
