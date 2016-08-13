// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coQueueRef.h"

class coLogicalDevice;

class coVulkanQueueRef : public coQueueRef
{
public:
	coVulkanQueueRef();

	coLogicalDevice* logicalDevice;
	coInt32 familyIndex;
	coInt32 queueIndex;
	VkQueue queue_vk;
};
