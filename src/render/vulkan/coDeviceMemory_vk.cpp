// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coDeviceMemory_vk.h"
#include "render/vulkan/coLogicalDevice_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult_f.h"

coDeviceMemory_vk::coDeviceMemory_vk()
	: deviceMemory_vk(VK_NULL_HANDLE)
	, logicalDevice_vk(nullptr)
{

}

coDeviceMemory_vk::~coDeviceMemory_vk()
{
	if (deviceMemory_vk != VK_NULL_HANDLE)
	{
		vkFreeMemory(logicalDevice_vk->GetVkDevice(), deviceMemory_vk, nullptr);
	}
}

coDeviceMemory_vk::InitConfig::InitConfig()
	: device_vk(nullptr)
	, size(0)
{

}

coResult coDeviceMemory_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	logicalDevice_vk = config.device_vk;
	coTRY(logicalDevice_vk, nullptr);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = config.size;
	allocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	coTRY_vk(vkAllocateMemory(logicalDevice_vk->GetVkDevice(), &allocInfo, nullptr, &deviceMemory_vk), "Failed to allocate memory");
	return true;
}
