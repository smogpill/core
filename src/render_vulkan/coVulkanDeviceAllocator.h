// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanDeviceAllocator : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:

private:
	class Block
	{
	public:
		VkDeviceMemory deviceMemory_vk;
	};
};
