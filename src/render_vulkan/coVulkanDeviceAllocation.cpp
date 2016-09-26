// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDeviceAllocation.h"
#include "render_vulkan/coVulkanDeviceAllocator.h"

coVulkanDeviceAllocation::coVulkanDeviceAllocation()
	: allocator(nullptr)
	, chunk(nullptr)
	, offset_vk(0)
	, size_vk(0)
{

}

coVulkanDeviceAllocation::~coVulkanDeviceAllocation()
{
	if (size_vk)
	{
		if (allocator)
		{
			allocator->Free(*this);
		}
	}
}
