// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coVulkanDeviceMemoryBlock
{
public:
	coVulkanDeviceMemoryBlock();

	VkDeviceSize size_vk;
	VkDeviceSize offset_vk;
	coBool isFree;
};