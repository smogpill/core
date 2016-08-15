// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanImage.h"
#include "render_vulkan/coVulkanImageView.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"

coVulkanImage::coVulkanImage()
	: image_vk(VK_NULL_HANDLE)
	, deviceMemory_vk(VK_NULL_HANDLE)
	, format_vk(VK_FORMAT_UNDEFINED)
	, imageType_vk(VK_IMAGE_TYPE_MAX_ENUM)
{

}

coVulkanImage::InitConfig::InitConfig()
	: deviceMemory_vk(VK_NULL_HANDLE)
	, deviceMemoryOffset_vk(0)
	, image_vk(VK_NULL_HANDLE)
	, format_vk(VK_FORMAT_UNDEFINED)
{

}

coVulkanImage::~coVulkanImage()
{
	delete defaultView;
	defaultView = nullptr;
	if (image_vk != VK_NULL_HANDLE)
	{
		if (deviceMemory_vk != VK_NULL_HANDLE)
		{
			const VkDevice& device_vk = GetVkDevice();
			coASSERT(device_vk);
			vkDestroyImage(device_vk, image_vk, nullptr);
		}
	}
}

coResult coVulkanImage::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk, nullptr);
	coTRY(config.arraySize > 0, nullptr);
	size = config.size;
	coASSERT(image_vk == VK_NULL_HANDLE);
	image_vk = config.image_vk;
	format_vk = config.format_vk;
	coTRY(InitImageType(), "Failed to init image type.");

	if (image_vk == VK_NULL_HANDLE)
	{
		format_vk = VK_FORMAT_R8G8B8A8_UNORM;
		VkImageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = imageType_vk;
		createInfo.extent.width = size.x;
		createInfo.extent.height = size.y;
		createInfo.extent.depth = size.z;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = config.arraySize;
		createInfo.format = format_vk;
		createInfo.tiling = VK_IMAGE_TILING_LINEAR;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		createInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.flags = 0;

		coVULKAN_TRY(vkCreateImage(device_vk, &createInfo, nullptr, &image_vk), "Failed to create Vulkan image.");
	}

	coTRY(InitDefaultView(), "Failed to init the default view of the image.");
	return true;
}

coResult coVulkanImage::InitDefaultView()
{
	coRenderImageView* view = new coVulkanImageView();
	coDEFER() { delete view; };
	coVulkanImageView::InitConfig c;
	c.device = device;
	c.image = this;
	c.debugName = GetDebugName();
	coTRY(view->Init(c), "Failed to init the default view.");
	coSwap(defaultView, view);
	return true;
}

coResult coVulkanImage::InitImageType()
{
	imageType_vk = VK_IMAGE_TYPE_MAX_ENUM;
	coTRY(size.x > 0, nullptr);
	coTRY(size.y > 0, nullptr);
	coTRY(size.z > 0, nullptr);

	// Compute dimensions
	coUint32 nbDimensions = 0;
	for (coUint i = 0; i < 3; ++i)
		if (size[i] > 1)
			++nbDimensions;

	switch (nbDimensions)
	{
	case 1: imageType_vk = VK_IMAGE_TYPE_1D; break;
	case 2: imageType_vk = VK_IMAGE_TYPE_2D; break;
	case 3: imageType_vk = VK_IMAGE_TYPE_3D; break;
	default:
	{
		coERROR("Invalid dimension count: " << nbDimensions);
		return false;
		break;
	}
	}

	return true;
}

const VkDevice& coVulkanImage ::GetVkDevice() const
{
	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice;
}
