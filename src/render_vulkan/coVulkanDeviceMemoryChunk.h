// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/list/coList.h"
#include "render_vulkan/coVulkanDeviceMemoryBlock.h"

class coVulkanDeviceAllocation;

class coVulkanDeviceMemoryChunk
{
public:
	coVulkanDeviceMemoryChunk();
	~coVulkanDeviceMemoryChunk();
	VkDeviceMemory deviceMemory_vk;
	VkMemoryPropertyFlags flags_vk;
	VkDeviceSize size_vk;
	coList<coVulkanDeviceMemoryBlock> blocks;
};
