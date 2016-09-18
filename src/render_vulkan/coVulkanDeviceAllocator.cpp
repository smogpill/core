// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDeviceAllocator.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanDeviceMemoryChunk.h"
#include "render_vulkan/coVulkanDeviceAllocation.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"
#include "container/list/coList_f.h"

coResult coVulkanDeviceAllocator::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

// 	coVulkanLogicalDevice* vulkanDevice = static_cast<coVulkanLogicalDevice*>(device);
// 	coTRY(vulkanDevice, nullptr);
// 	const coVulkanPhysicalDevice* vulkanPhysicalDevice = vulkanDevice->GetPhysicalDevice();
// 	coTRY(vulkanPhysicalDevice, nullptr);
// 	const VkPhysicalDeviceMemoryProperties& memProps_vk = vulkanPhysicalDevice->GetMemoryProperties();
	
// 	VkMemoryAllocateInfo allocInfo{};
// 	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
// 	allocInfo.allocationSize = 256 * 1024 * 1024;
// 	allocInfo.memoryTypeIndex = ;
// 
// 	const VkDevice& device_vk = GetVkDevice();
// 	VkDeviceMemory deviceMemory_vk;
// 	coVULKAN_TRY(vkAllocateMemory(device_vk, &allocInfo, nullptr, &deviceMemory_vk), "Failed to allocate device memory.");

	return true;
}

const VkDevice& coVulkanDeviceAllocator::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

coResult coVulkanDeviceAllocator::Allocate(coVulkanDeviceAllocation*& /*_alloc*/, const VkMemoryPropertyFlags& _flags_vk, const VkDeviceSize& _size_vk)
{
	// Initial raw implementation from https://cpp-rendering.io/vulkan-and-pipelines/. 

	VkDeviceSize size_vk = _size_vk;
	if (size_vk % 128 != 0)
		size_vk = size_vk + (128 - (size_vk % 128)); // 128 bytes alignment
	coASSERT(size_vk % 128 == 0);

	// Find chunk
	for (coVulkanDeviceMemoryChunk* chunk : chunks)
	{
		coASSERT(chunk);
		if ((chunk->flags_vk & _flags_vk) == _flags_vk)
		{
			// Find block
			coVulkanDeviceMemoryBlock* selectedBlock = nullptr;
			for (auto& block : chunk->blocks)
			{
				if (block.isFree)
				{
					if (block.size_vk > size_vk)
					{
						selectedBlock = &block;
						break;
					}
				}
			}

			if (!!selectedBlock)
				continue;

			coVulkanDeviceMemoryBlock newBlock;
			newBlock.isFree = true;
			newBlock.offset_vk = selectedBlock->offset_vk + size_vk;
			newBlock.size_vk = selectedBlock->size_vk - size_vk;

			selectedBlock->isFree = false;
			selectedBlock->size_vk = size_vk;

			if (newBlock.size_vk != 0)
			{
				coWARN_NOT_AVAILABLE();
				//coInsertAfter(*selectedBlock, new coListNodeData<coVulkanDeviceMemoryBlock>(newBlock));
			}
		}
	}

	coVulkanDeviceMemoryChunk* chunk = nullptr;
	coTRY(AllocateChunk(chunk, _flags_vk, 256 * 1024 * 1024), "Failed to allocate new Vulkan memory chunk.");
	coASSERT(chunk);

	return true;
}

void coVulkanDeviceAllocator::Free(coVulkanDeviceAllocation& _alloc)
{
	for (coVulkanDeviceMemoryBlock& block : _alloc.chunk->blocks)
	{
		if (block.offset_vk == _alloc.offset_vk)
		{
			block.isFree = true;
			return;
		}
	}
	coASSERT(false);
}

coResult coVulkanDeviceAllocator::AllocateChunk(coVulkanDeviceMemoryChunk*& /*_chunk*/, const VkMemoryPropertyFlags& _requiredFlags_vk, const VkDeviceSize& _size_vk)
{
	coVulkanLogicalDevice* vulkanDevice = static_cast<coVulkanLogicalDevice*>(device);
	coTRY(vulkanDevice, nullptr);
	const coVulkanPhysicalDevice* vulkanPhysicalDevice = vulkanDevice->GetPhysicalDevice();
	coTRY(vulkanPhysicalDevice, nullptr);
	const VkPhysicalDeviceMemoryProperties& memProps_vk = vulkanPhysicalDevice->GetMemoryProperties();

	// Find heap
	coInt index = -1;
	{
		for (coUint i = 0; i < memProps_vk.memoryTypeCount; ++i)
		{
			if ((memProps_vk.memoryTypes[i].propertyFlags & _requiredFlags_vk) == _requiredFlags_vk)
			{
				const VkMemoryHeap& heap_vk = memProps_vk.memoryHeaps[memProps_vk.memoryTypes[i].heapIndex];
				if (_size_vk < heap_vk.size)
				{
					index = i;
				}
			}
		}
		coTRY(index == -1, "Failed to find a valid Vulkan memory heap for the allocation.");
	}

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = _size_vk;
	allocInfo.memoryTypeIndex = index;

	const VkDevice& device_vk = GetVkDevice();
	VkDeviceMemory deviceMemory_vk;
	coVULKAN_TRY(vkAllocateMemory(device_vk, &allocInfo, nullptr, &deviceMemory_vk), "Failed to allocate device memory.");
	coTRY(deviceMemory_vk != VK_NULL_HANDLE, "");

	const VkMemoryType& memoryType_vk = memProps_vk.memoryTypes[index];

	coVulkanDeviceMemoryChunk* chunk = new coVulkanDeviceMemoryChunk();
	chunk->deviceMemory_vk = deviceMemory_vk;
	chunk->flags_vk = memoryType_vk.propertyFlags;
	chunk->size_vk = _size_vk;
	coPushBack(chunks, chunk);

	return true;
}

coVulkanDeviceAllocator::~coVulkanDeviceAllocator()
{
	for (auto& p : chunks)
		delete p;
}
