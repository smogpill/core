// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanImageView.h"
#include "render_vulkan/coVulkanImage.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanImageView::coVulkanImageView()
	: imageView_vk(VK_NULL_HANDLE)
{

}

coVulkanImageView::~coVulkanImageView()
{
	if (imageView_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk);
		vkDestroyImageView(device_vk, imageView_vk, nullptr);
	}
}

coResult coVulkanImageView::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk, nullptr);
	const VkImage& image_vk = GetVkImage();
	coTRY(image_vk, nullptr);

	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image_vk;
	coTRY(ComputeType(createInfo.viewType, config), nullptr);
	coTRY(ComputeFormat(createInfo.format, config), nullptr);
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	coASSERT(imageView_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateImageView(device_vk, &createInfo, nullptr, &imageView_vk), "Failed to create Vulkan image view.");
	return true;
}

const VkDevice& coVulkanImageView::GetVkDevice() const
{
	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice;
}

const VkImage& coVulkanImageView::GetVkImage() const
{
	coVulkanImage* vulkanImage = static_cast<coVulkanImage*>(image);
	static VkImage nullImage = VK_NULL_HANDLE;
	return vulkanImage ? vulkanImage->GetVkImage() : nullImage;
}

coResult coVulkanImageView::ComputeType(VkImageViewType& _out, const InitConfig& _config)
{
	_out = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
	switch (_config.type)
	{
	case tex1D: _out = VK_IMAGE_VIEW_TYPE_1D; break;
	case tex2D: _out = VK_IMAGE_VIEW_TYPE_2D; break;
	case tex3D: _out = VK_IMAGE_VIEW_TYPE_3D; break;
	case texCube: _out = VK_IMAGE_VIEW_TYPE_CUBE; break;
	case default:
	{
		coVulkanImage* vulkanImage = static_cast<coVulkanImage*>(image);
		coTRY(vulkanImage, nullptr);
		const VkImageType& imageType_vk = vulkanImage->GetVkImageType();
		switch (imageType_vk)
		{
		case VK_IMAGE_TYPE_1D: _out = VK_IMAGE_VIEW_TYPE_1D; break;
		case VK_IMAGE_TYPE_2D: _out = VK_IMAGE_VIEW_TYPE_2D; break;
		case VK_IMAGE_TYPE_3D: _out = VK_IMAGE_VIEW_TYPE_3D; break;
		default: coASSERT(false); return false;
		}
		break;
	}
	default: coASSERT(false); return false;
	}
	return true;
}

coResult coVulkanImageView::ComputeFormat(VkFormat& _out, const InitConfig& /*_config*/)
{
	_out = VK_FORMAT_UNDEFINED;
	coVulkanImage* vulkanImage = static_cast<coVulkanImage*>(image);
	coTRY(vulkanImage, nullptr);
	_out = vulkanImage->GetVkFormat();
	return true;
}