// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coVulkanDeviceMemoryChunk;
class coVulkanDeviceAllocator;

class coVulkanDeviceAllocation
{
public:
	coVulkanDeviceAllocation();
	~coVulkanDeviceAllocation();
	coVulkanDeviceAllocator* allocator;
	coVulkanDeviceMemoryChunk* chunk;
	VkDeviceSize offset_vk;
	VkDeviceSize size_vk;
};