// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDeviceAllocation.h"

coVulkanDeviceAllocation::coVulkanDeviceAllocation()
	: chunk(nullptr)
	, offset_vk(0)
	, size_vk(0)
{

}