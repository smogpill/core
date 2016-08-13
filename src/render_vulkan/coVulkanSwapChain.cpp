// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render/coSwapChain.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "math/scalar/coUint32_f.h"

coVulkanSwapChain::coVulkanSwapChain()
	: swapChain_vk(VK_NULL_HANDLE)
{

}

coVulkanSwapChain::~coVulkanSwapChain()
{
	if (swapChain_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		vkDestroySwapchainKHR(device_vk, swapChain_vk, nullptr);
	}
}

coResult coVulkanSwapChain::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(logicalDevice);
	coTRY(vulkanLogicalDevice, nullptr);
	const VkDevice& device_vk = vulkanLogicalDevice->GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	const VkPhysicalDevice& physicalDevice_vk = GetVkPhysicalDevice();
	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);

	const VkSurfaceKHR& surface_vk = GetVkSurfaceKHR();
	coTRY(surface_vk != VK_NULL_HANDLE, nullptr);

	VkSurfaceCapabilitiesKHR capabilities;
	coVULKAN_TRY(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice_vk, surface_vk, &capabilities), "Failed to get surface capabilities");

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface_vk;

	// Select format
	{
		VkSurfaceFormatKHR requestedFormat = {};
		requestedFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		requestedFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		VkSurfaceFormatKHR format;
		coTRY(SelectFormat(format, requestedFormat), "Failed to select a format.");
		createInfo.imageFormat = format.format;
		createInfo.imageColorSpace = format.colorSpace;
	}

	// Select present mode
	{
		VkPresentModeKHR requestedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		coTRY(SelectPresentMode(createInfo.presentMode, requestedPresentMode), "Failed to select a present mode.");
	}

	// Select extent
	{
		VkExtent2D requestedExtent;
		requestedExtent.width = config.size.x;
		requestedExtent.height = config.size.y;
		coTRY(SelectExtent(createInfo.imageExtent, capabilities, requestedExtent), "Failed to select an extent.");
	}


	createInfo.minImageCount = config.nbImages;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const coInt32 graphicsFamilyIndex = coVulkanLogicalDevice::GetQueueFamilyIndex(vulkanLogicalDevice->GetQueueId(coVulkanLogicalDevice::graphics));
	const coInt32 presentFamilyIndex = coVulkanLogicalDevice::GetQueueFamilyIndex(vulkanLogicalDevice->GetQueueId(coVulkanLogicalDevice::present));
	coTRY(graphicsFamilyIndex != -1, nullptr);
	coTRY(presentFamilyIndex != -1, nullptr);
	coDynamicArray<coUint32> queueFamilyIndices;
	if (graphicsFamilyIndex != presentFamilyIndex)
	{
		coPushBack(queueFamilyIndices, graphicsFamilyIndex);
		coPushBack(queueFamilyIndices, presentFamilyIndex);
	}
	createInfo.imageSharingMode = queueFamilyIndices.count > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = queueFamilyIndices.count;
	createInfo.pQueueFamilyIndices = queueFamilyIndices.data;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped = VK_TRUE;
	coVulkanSwapChain*  oldSwapChain_vk = static_cast<coVulkanSwapChain*>(config.oldSwapChain);
	createInfo.oldSwapchain = oldSwapChain_vk ? oldSwapChain_vk->swapChain_vk : VK_NULL_HANDLE;

	coTRY(swapChain_vk == VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkCreateSwapchainKHR(GetVkDevice(), &createInfo, nullptr, &swapChain_vk), "Failed to create swap chain.");

	return true;
}


coResult coVulkanSwapChain::GetFormats(coDynamicArray<VkSurfaceFormatKHR>& _out)
{
	coClear(_out);

	const VkPhysicalDevice& physicalDevice_vk = GetVkPhysicalDevice();
	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	const VkSurfaceKHR& surface_vk = GetVkSurfaceKHR();
	coTRY(surface_vk != VK_NULL_HANDLE, nullptr);

	coUint32 nb;
	coVULKAN_TRY(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_vk, surface_vk, &nb, nullptr), "Failed to retrieve the supported formats count.");

	coResize(_out, nb);
	coVULKAN_TRY(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_vk, surface_vk, &_out.count, _out.data), "Failed to retrieve the supported formats.");
	coTRY(_out.count == nb, nullptr);
	return true;
}

coResult coVulkanSwapChain::GetPresentModes(coDynamicArray<VkPresentModeKHR>& _out)
{
	coClear(_out);

	const VkPhysicalDevice& physicalDevice_vk = GetVkPhysicalDevice();
	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	const VkSurfaceKHR& surface_vk = GetVkSurfaceKHR();
	coTRY(surface_vk != VK_NULL_HANDLE, nullptr);

	coUint32 nb;
	coVULKAN_TRY(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice_vk, surface_vk, &nb, nullptr), "Failed to retrieve the present modes count.");

	coResize(_out, nb);
	coVULKAN_TRY(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice_vk, surface_vk, &_out.count, _out.data), "Failed to retrieve the present modes.");
	coTRY(_out.count == nb, nullptr);
	return true;
}

coResult coVulkanSwapChain::SelectFormat(VkSurfaceFormatKHR& _out, const VkSurfaceFormatKHR& _requested)
{

	coDynamicArray<VkSurfaceFormatKHR> supportedFormats;
	coTRY(GetFormats(supportedFormats), "Failed to get the supported formats.");

	// Check if it supports all formats
	if (supportedFormats.count == 1 && supportedFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		_out = _requested;
		return true;
	}

	// Find the requested format
	for (const VkSurfaceFormatKHR& f : supportedFormats)
	{
		if (f.format == _requested.format && f.colorSpace == _requested.colorSpace)
		{
			_out = _requested;
			return true;
		}
	}

	// If not the requested format, we return the first supported format.
	coTRY(supportedFormats.count, nullptr);
	_out = supportedFormats[0];

	return true;
}

coResult coVulkanSwapChain::SelectPresentMode(VkPresentModeKHR& _out, const VkPresentModeKHR& _requested)
{
	coDynamicArray<VkPresentModeKHR> supportedPresentModes;
	coTRY(GetPresentModes(supportedPresentModes), "Failed to get the supported present modes.");

	// Find the requested present mode
	for (const VkPresentModeKHR& m : supportedPresentModes)
	{
		if (m == _requested)
		{
			_out = _requested;
			return true;
		}
	}

	_out = VK_PRESENT_MODE_FIFO_KHR;
	return true;
}

coResult coVulkanSwapChain::SelectExtent(VkExtent2D& _out, const VkSurfaceCapabilitiesKHR& _capabilities, const VkExtent2D& _requested)
{
	typedef decltype(_capabilities.currentExtent.width) Type;
	if (_capabilities.currentExtent.width != coNumericLimits<Type>::Max())
	{
		_out = _capabilities.currentExtent;
	}
	else
	{
		_out.width = coMax(_capabilities.minImageExtent.width, coMin(_capabilities.maxImageExtent.width, _requested.width));
		_out.height = coMax(_capabilities.minImageExtent.height, coMin(_capabilities.maxImageExtent.height, _requested.height));
	}

	return true;
}

const VkDevice& coVulkanSwapChain::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(logicalDevice);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

const VkPhysicalDevice& coVulkanSwapChain::GetVkPhysicalDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(logicalDevice);
	coVulkanPhysicalDevice* vulkanPhysicalDevice = vulkanLogicalDevice ? vulkanLogicalDevice->GetPhysicalDevice() : nullptr;
	static VkPhysicalDevice nullPhysicalDevice_vk = VK_NULL_HANDLE;
	return vulkanPhysicalDevice ? vulkanPhysicalDevice->GetVkPhysicalDevice() : nullPhysicalDevice_vk;
}

const VkSurfaceKHR& coVulkanSwapChain::GetVkSurfaceKHR() const
{
	coVulkanSurface* vulkanSurface = static_cast<coVulkanSurface*>(surface);
	static VkSurfaceKHR nullSurface_vk = VK_NULL_HANDLE;
	return vulkanSurface ? vulkanSurface->GetVkSurfaceKHR() : nullSurface_vk;
}
