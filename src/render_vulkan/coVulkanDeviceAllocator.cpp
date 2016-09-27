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

coResult coVulkanDeviceAllocator::Allocate(coVulkanDeviceAllocation*& _alloc, const VkMemoryPropertyFlags& _flags_vk, const VkDeviceSize& _size_vk)
{
	delete _alloc;
	_alloc = nullptr;

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
			coList<coVulkanDeviceMemoryBlock>::Entry* selectedBlockEntry = nullptr;
			for (auto& entry : chunk->blocks)
			{
				const auto& block = entry.data;
				if (block.isFree)
				{
					if (block.size_vk > size_vk)
					{
						selectedBlockEntry = &entry;
						break;
					}
				}
			}

			if (!selectedBlockEntry)
				continue;

			coVulkanDeviceMemoryBlock newBlock;
			newBlock.isFree = true;
			newBlock.offset_vk = selectedBlockEntry->data.offset_vk + size_vk;
			newBlock.size_vk = selectedBlockEntry->data.size_vk - size_vk;

			selectedBlockEntry->data.isFree = false;
			selectedBlockEntry->data.size_vk = size_vk;

			if (newBlock.size_vk != 0)
			{
				coInsertAfter(*selectedBlockEntry, newBlock);
			}

			coVulkanDeviceAllocation* allocation = new coVulkanDeviceAllocation();
			allocation->allocator = this;
			allocation->chunk = chunk;
			allocation->offset_vk = selectedBlockEntry->data.offset_vk;
			allocation->size_vk = size_vk;
			coASSERT(!_alloc);
			_alloc = allocation;
			return true;
		}
	}

	coVulkanDeviceMemoryChunk* chunk = nullptr;
	coTRY(AllocateChunk(chunk, _flags_vk, 256 * 1024 * 1024), "Failed to allocate new Vulkan memory chunk.");
	coASSERT(chunk);

	return Allocate(_alloc, _flags_vk, _size_vk);
}

void coVulkanDeviceAllocator::Free(coVulkanDeviceAllocation& _alloc)
{
	for (auto& entry : _alloc.chunk->blocks)
	{
		auto& block = entry.data;
		if (block.offset_vk == _alloc.offset_vk)
		{
			block.isFree = true;
			return;
		}
	}
	coASSERT(false);
}

coResult coVulkanDeviceAllocator::AllocateChunk(coVulkanDeviceMemoryChunk*& _chunk, const VkMemoryPropertyFlags& _requiredFlags_vk, const VkDeviceSize& _size_vk)
{
	_chunk = nullptr;
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
			const VkMemoryType& memoryType_vk = memProps_vk.memoryTypes[i];
			if ((memoryType_vk.propertyFlags & _requiredFlags_vk) == _requiredFlags_vk)
			{
				const VkMemoryHeap& heap_vk = memProps_vk.memoryHeaps[memoryType_vk.heapIndex];
				if (_size_vk < heap_vk.size)
				{
					index = i;
					break;
				}
			}
		}
		coTRY(index != -1, "Failed to find a valid Vulkan memory heap for the allocation.");
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

	_chunk = new coVulkanDeviceMemoryChunk();
	_chunk->deviceMemory_vk = deviceMemory_vk;
	_chunk->flags_vk = memoryType_vk.propertyFlags;
	_chunk->size_vk = _size_vk;
	
	coVulkanDeviceMemoryBlock block;
	block.isFree = true;
	block.offset_vk = 0;
	block.size_vk = _size_vk;
	coPushBack(_chunk->blocks, block);

	coPushBack(chunks, _chunk);

	return true;
}

coVulkanDeviceAllocator::~coVulkanDeviceAllocator()
{
	const VkDevice& device_vk = GetVkDevice();
	for (auto& p : chunks)
	{
		coCHECK(device_vk != VK_NULL_HANDLE, nullptr);
		vkFreeMemory(device_vk, p->deviceMemory_vk, nullptr);
		delete p;
	}
}
