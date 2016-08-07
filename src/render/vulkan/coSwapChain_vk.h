// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coSurface_vk;
class coLogicalDevice_vk;

class coSwapChain_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coSurface_vk* surface_vk;
		coLogicalDevice_vk* logicalDevice_vk;
		coSwapChain_vk* oldSwapChain_vk;
		coUint32 width;
		coUint32 height;
	};

	coSwapChain_vk();
	virtual ~coSwapChain_vk();
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult SelectFormat(VkSurfaceFormatKHR& _out, const VkSurfaceFormatKHR& _requested);
	coResult SelectPresentMode(VkPresentModeKHR& _out, const VkPresentModeKHR& _requested);
	coResult SelectExtent(VkExtent2D& _out, const VkSurfaceCapabilitiesKHR& _capabilities, const VkExtent2D& _requested);
	coResult GetFormats(coDynamicArray<VkSurfaceFormatKHR>& _out);
	coResult GetPresentModes(coDynamicArray<VkPresentModeKHR>& _out);

	coLogicalDevice_vk* logicalDevice_vk;
	coSurface_vk* surface_vk;
	VkSwapchainKHR swapChain_vk;
	coUint32 nbImages;
	VkFormat imageFormat_vk;
	VkExtent2D extent_vk;
	VkColorSpaceKHR colorSpace_vk;
};