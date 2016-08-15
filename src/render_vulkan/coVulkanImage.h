// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderImage.h"

class coVulkanLogicalDevice;

class coVulkanImage : public coRenderImage
{
	coDECLARE_SUPER(coRenderImage);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		VkDeviceMemory deviceMemory_vk;
		VkDeviceSize deviceMemoryOffset_vk;
		VkImage image_vk;
		VkFormat format_vk;
	};
	coVulkanImage();
	virtual ~coVulkanImage();

	//coResult InitFromVkImage(const VkImage& _image_vk, coRenderDevice& _device, const VkFormat& _format_vk, const coInt32x3& _size);
	const VkImage& GetVkImage() const { return image_vk; }
	const VkFormat& GetVkFormat() const { return format_vk; }
	const VkImageType& GetVkImageType() const { return imageType_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitImageType();
	coResult InitDefaultView();
	const VkDevice& GetVkDevice() const;

	VkDeviceMemory deviceMemory_vk;
	VkImage image_vk;
	VkFormat format_vk;
	VkImageType imageType_vk;
};
