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
	};
	coVulkanImage();
	virtual ~coVulkanImage();

	coResult InitFromVkImage(const VkImage& _image_vk);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	static coResult ComputeImageType(VkImageType& _out, const InitConfig& _config);
	const VkDevice& GetVkDevice() const;

	VkDeviceMemory deviceMemory_vk;
	VkImage image_vk;
};
