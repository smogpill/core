// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDeviceMemoryChunk.h"
#include "render_vulkan/coVulkanResult_f.h"

coVulkanDeviceMemoryChunk::coVulkanDeviceMemoryChunk()
	: deviceMemory_vk(VK_NULL_HANDLE)
	, flags_vk(0)
	, size_vk(0)
{

}

coVulkanDeviceMemoryChunk::~coVulkanDeviceMemoryChunk()
{
}
