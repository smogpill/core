// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coImage_vk.h"
#include "render/vulkan/coLogicalDevice_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult_f.h"

coImage_vk::coImage_vk()
	: image_vk(VK_NULL_HANDLE)
	, device_vk(nullptr)
	, deviceMemory_vk(VK_NULL_HANDLE)
{

}

coImage_vk::InitConfig::InitConfig()
	: device_vk(nullptr)
	, size(0)
	, type(Type::default)
	, arraySize(1)
	, deviceMemory_vk(VK_NULL_HANDLE)
	, deviceMemoryOffset_vk(0)
{

}

coImage_vk::~coImage_vk()
{
	if (image_vk != VK_NULL_HANDLE)
	{
		coASSERT(device_vk);
		coASSERT(device_vk->GetVkDevice() != VK_NULL_HANDLE);
		vkDestroyImage(device_vk->GetVkDevice(), image_vk, nullptr);
	}
}

coResult coImage_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	device_vk = config.device_vk;
	coTRY(device_vk, nullptr);
	coTRY(config.arraySize > 0, nullptr);

	VkImageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	coTRY(ComputeImageType(createInfo.imageType, config), nullptr);
	createInfo.extent.width = config.size.x;
	createInfo.extent.height = config.size.y;
	createInfo.extent.depth = config.size.z;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = config.arraySize;
	createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	createInfo.tiling = VK_IMAGE_TILING_LINEAR;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	createInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.flags = 0;

	coASSERT(device_vk->GetVkDevice() != VK_NULL_HANDLE);
	coTRY_vk(vkCreateImage(device_vk->GetVkDevice(), &createInfo, nullptr, &image_vk), "Failed to create image.");
	return true;
}

coResult coImage_vk::ComputeImageType(VkImageType& _out, const InitConfig& _config)
{
	coTRY(_config.size.x > 0, nullptr);
	coTRY(_config.size.y > 0, nullptr);
	coTRY(_config.size.z > 0, nullptr);

	// Compute dimensions
	coUint32 nbDimensions = 0;
	for (coUint i = 0; i < 3; ++i)
		if (_config.size[i] > 1)
			++nbDimensions;

	switch (nbDimensions)
	{
	case 1: _out = VK_IMAGE_TYPE_1D; break;
	case 2: _out = VK_IMAGE_TYPE_2D; break;
	case 3: _out = VK_IMAGE_TYPE_3D; break;
	default:
	{
		_out = VK_IMAGE_TYPE_1D;
		coERROR("Invalid dimension count: " << nbDimensions);
		return false;
		break;
	}
	}

	return true;
}