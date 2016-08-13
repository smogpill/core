// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanQueueRef.h"

coVulkanQueueRef::coVulkanQueueRef()
	: logicalDevice(nullptr)
	, familyIndex(-1)
	, queueIndex(-1)
	, queue_vk(VK_NULL_HANDLE)
{

}
