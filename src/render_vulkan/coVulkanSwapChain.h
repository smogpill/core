// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coSwapChain.h"

class coVulkanLogicalDevice;

class coVulkanSwapChain final : public coSwapChain
{
	coDECLARE_SUPER(coSwapChain);
public:
	coVulkanSwapChain();
	virtual ~coVulkanSwapChain();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult GetFormats(coDynamicArray<VkSurfaceFormatKHR>& _out);
	coResult GetPresentModes(coDynamicArray<VkPresentModeKHR>& _out);
	coResult SelectFormat(VkSurfaceFormatKHR& _out, const VkSurfaceFormatKHR& _requested);
	coResult SelectPresentMode(VkPresentModeKHR& _out, const VkPresentModeKHR& _requested);
	coResult SelectExtent(VkExtent2D& _out, const VkSurfaceCapabilitiesKHR& _capabilities, const VkExtent2D& _requested);

	VkSwapchainKHR swapChain_vk;
	coVulkanLogicalDevice* vulkanLogicalDevice;
};
