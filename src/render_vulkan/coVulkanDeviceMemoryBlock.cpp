// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDeviceMemoryBlock.h"

coVulkanDeviceMemoryBlock::coVulkanDeviceMemoryBlock()
	: size_vk(0)
	, offset_vk(0)
	, isFree(false)
{

}