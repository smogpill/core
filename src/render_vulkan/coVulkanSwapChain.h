// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coSwapChain.h"

class coVulkanPhysicalDevice;

class coVulkanSwapChain final : public coSwapChain
{
	coDECLARE_SUPER(coSwapChain);
public:
	coVulkanSwapChain();
	virtual ~coVulkanSwapChain();

	virtual coResult AcquireImage();
	virtual coResult Present(const coArray<coRenderSemaphore*> _waitSemaphores) override;
	const VkSwapchainKHR& GetVkSwapchainKHR() const { return swapChain_vk; }
	coInt32 GetCurrentImageIndex() const { return currentImageIndex; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult GetFormats(coDynamicArray<VkSurfaceFormatKHR>& _out);
	coResult GetPresentModes(coDynamicArray<VkPresentModeKHR>& _out);
	coResult InitImages();
	coResult InitSemaphores();
	coResult SelectFormat(VkSurfaceFormatKHR& _out, const VkSurfaceFormatKHR& _requested);
	coResult SelectPresentMode(VkPresentModeKHR& _out, const VkPresentModeKHR& _requested);
	coResult SelectExtent(VkExtent2D& _out, const VkSurfaceCapabilitiesKHR& _capabilities, const VkExtent2D& _requested);
	coResult FindGraphicsQueueFamily(coInt32& _queueFamily) const;
	coResult FindPresentQueueFamily(coInt32& _queueFamily) const;
	const coVulkanPhysicalDevice* GetVulkanPhysicalDevice() const;
	const VkDevice& GetVkDevice() const;
	const VkPhysicalDevice& GetVkPhysicalDevice() const;
	const VkSurfaceKHR& GetVkSurfaceKHR() const;

	VkSwapchainKHR swapChain_vk;
	VkQueue presentQueue_vk;
	coInt32 currentImageIndex;
	VkFormat format_vk;
	VkExtent2D extent2D_vk;
};
