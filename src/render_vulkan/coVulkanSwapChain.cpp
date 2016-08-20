// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanImage.h"
#include "render_vulkan/coVulkanImageView.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanSemaphore.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanPass.h"
#include "render/coSwapChain.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "math/scalar/coUint32_f.h"
#include "pattern/scope/coDefer.h"

coVulkanSwapChain::coVulkanSwapChain()
	: swapChain_vk(VK_NULL_HANDLE)
	, currentImageIndex(-1)
	, presentQueue_vk(VK_NULL_HANDLE)
	, format_vk(VK_FORMAT_UNDEFINED)
	, extent2D_vk{}
{

}

coVulkanSwapChain::~coVulkanSwapChain()
{
	delete renderPass;
	renderPass = nullptr;
	delete imageAvailableSemaphore;
	imageAvailableSemaphore = nullptr;

	for (auto& p : imageViews)
		delete p;
	coClear(imageViews);
	for (auto& p : images)
		delete p;
	coClear(images);
	for (auto& p : frameBuffers)
		delete p;
	coClear(frameBuffers);

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

	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(device);
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
		format_vk = format.format;
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
		coTRY(SelectExtent(extent2D_vk, capabilities, requestedExtent), "Failed to select an extent.");
		createInfo.imageExtent = extent2D_vk;
	}

	createInfo.minImageCount = config.nbImages;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	coInt32 graphicsFamilyIndex;
	coTRY(FindGraphicsQueueFamily(graphicsFamilyIndex), "Failed to find a device queue compatible with graphics.");
	coInt32 presentFamilyIndex;
	coTRY(FindPresentQueueFamily(presentFamilyIndex), "Failed to find a device queue compatible with presentation.");
	coTRY(vulkanLogicalDevice->GetVkQueue(presentQueue_vk, presentFamilyIndex, 0), "Failed to retrieve the present queue of family index: "<<presentFamilyIndex);
	coTRY(presentQueue_vk != VK_NULL_HANDLE, nullptr);
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
	coVULKAN_TRY(vkCreateSwapchainKHR(device_vk, &createInfo, nullptr, &swapChain_vk), "Failed to create swap chain.");

	coTRY(InitImages(), "Failed to init images.");
	coTRY(InitImageViews(), "Failed to init image views.");
	coTRY(InitSemaphores(), "Failed to init semaphores.");
	coTRY(InitPass(), "Failed to init pass.");
	coTRY(InitFramebuffers(), "Failed to init framebuffers.");

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
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

const VkPhysicalDevice& coVulkanSwapChain::GetVkPhysicalDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
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

coResult coVulkanSwapChain::AcquireImage()
{
	coTRY(Super::AcquireImage(), nullptr);
	coTRY(swapChain_vk != VK_NULL_HANDLE, nullptr);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	coVulkanSemaphore* vulkanSemaphore = static_cast<coVulkanSemaphore*>(imageAvailableSemaphore);
	coTRY(vulkanSemaphore, nullptr);
	const VkSemaphore& semaphore_vk = vulkanSemaphore->GetVkSemaphore();
	coTRY(semaphore_vk != VK_NULL_HANDLE, nullptr);

	coUint32 imageIndex;
	coVULKAN_TRY(vkAcquireNextImageKHR(device_vk, swapChain_vk, std::numeric_limits<uint64_t>::max(), semaphore_vk, VK_NULL_HANDLE, &imageIndex), "Failed to acquire next image");
	currentImageIndex = imageIndex;

	coTODO("Handle swap chain image acquisition specific messages.");
// 	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
// 		recreateSwapChain();
// 		return;
// 	}
// 	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
// 		throw std::runtime_error("failed to acquire swap chain image!");
// 	}
	return true;
}

coResult coVulkanSwapChain::Present(const coArray<coRenderSemaphore*> _waitSemaphores)
{
	coTRY(Super::Present(_waitSemaphores), nullptr);
	coTRY(currentImageIndex >= 0, nullptr);

	coDynamicArray<VkSemaphore> waitSemaphores_vk;
	coReserve(waitSemaphores_vk, _waitSemaphores.count);
	for (coRenderSemaphore* semaphore : _waitSemaphores)
	{
		coVulkanSemaphore* vulkanSemaphore = static_cast<coVulkanSemaphore*>(semaphore);
		coTRY(vulkanSemaphore, nullptr);
		const VkSemaphore& semaphore_vk = vulkanSemaphore->GetVkSemaphore();
		coTRY(semaphore_vk, nullptr);
		coPushBack(waitSemaphores_vk, semaphore_vk);
	}

	coUint32 imageIndex = currentImageIndex;
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = waitSemaphores_vk.count;
	presentInfo.pWaitSemaphores = waitSemaphores_vk.data;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain_vk;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;
	coTRY(imageIndex >= 0, nullptr);
	coTRY(presentQueue_vk != VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkQueuePresentKHR(presentQueue_vk, &presentInfo), "Failed to present the rendered images into swap chains");
	currentImageIndex = -1;

	return true;
}

coResult coVulkanSwapChain::FindGraphicsQueueFamily(coInt32& _queueFamily) const
{
	_queueFamily = -1;
	const coVulkanPhysicalDevice* vulkanPhysicalDevice = GetVulkanPhysicalDevice();
	coTRY(vulkanPhysicalDevice, nullptr);

	const coUint nbQueueFamilies = vulkanPhysicalDevice->GetNbQueueFamilies();
	for (coUint i = 0; i < nbQueueFamilies; ++i)
	{
		coBool b;
		coTRY(vulkanPhysicalDevice->SupportsSurface(b, i, *surface), nullptr);
		if (b)
		{
			_queueFamily = i;
			return true;
		}
	}

	return false;
}

coResult coVulkanSwapChain::FindPresentQueueFamily(coInt32& _queueFamily) const
{
	_queueFamily = -1;
	const coVulkanPhysicalDevice* vulkanPhysicalDevice = GetVulkanPhysicalDevice();
	coTRY(vulkanPhysicalDevice, nullptr);
	coTRY(surface, nullptr);

	const coUint nbQueueFamilies = vulkanPhysicalDevice->GetNbQueueFamilies();
	for (coUint i = 0; i < nbQueueFamilies; ++i)
	{
		coBool b;
		coTRY(vulkanPhysicalDevice->SupportsSurface(b, i, *surface), nullptr);
		if (b)
		{
			_queueFamily = i;
			return true;
		}
	}

	return false;
}

const coVulkanPhysicalDevice* coVulkanSwapChain::GetVulkanPhysicalDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetPhysicalDevice() : nullptr;
}

coResult coVulkanSwapChain::InitImages()
{
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	coTRY(swapChain_vk != VK_NULL_HANDLE, nullptr);
	coUint32 nbImages;
	coVULKAN_TRY(vkGetSwapchainImagesKHR(device_vk, swapChain_vk, &nbImages, nullptr), "Failed to retrieve the swap chain image count.");
	coDynamicArray<VkImage> images_vk;
	coResize(images_vk, nbImages);
	coVULKAN_TRY(vkGetSwapchainImagesKHR(device_vk, swapChain_vk, &images_vk.count, images_vk.data), "Failed to retrieve the swap chain images.");
	coTRY(images_vk.count == nbImages, nullptr);

	coASSERT(images.count == 0);
	coReserve(images, images_vk.count);
	coInt32x3 size;
	size.x = extent2D_vk.width;
	size.y = extent2D_vk.height;
	size.z = 1;
	for (const VkImage& image_vk : images_vk)
	{
		coASSERT(image_vk != VK_NULL_HANDLE);
		coVulkanImage* vulkanImage = new coVulkanImage();
		coDEFER() { delete vulkanImage; };
		coVulkanImage::InitConfig c;
		c.device = device;
		c.image_vk = image_vk;
		c.format_vk = format_vk;
		c.size = size;
		coTRY(vulkanImage->Init(c), nullptr);
		coPushBack(images, vulkanImage);
		vulkanImage = nullptr;
	}
	return true;
}

coResult coVulkanSwapChain::InitImageViews()
{
	coASSERT(!imageViews.count);
	coReserve(imageViews, images.count);
	for (coRenderImage* image : images)
	{
		coASSERT(image);
		coRenderImageView* imageView = new coVulkanImageView();
		coDEFER() { delete imageView; };
		coRenderImageView::InitConfig c;
		c.device = device;
		c.image = image;
		coTRY(imageView->Init(c), "Failed to init image view.");
		coPushBack(imageViews, imageView);
		imageView = nullptr;
	}
	return true;
}

coResult coVulkanSwapChain::InitSemaphores()
{
	coRenderSemaphore* semaphore = new coVulkanSemaphore();
	coDEFER() { delete semaphore; };
	coRenderSemaphore::InitConfig c;
	c.device = device;
	c.debugName = "imageAvailableSemaphore";
	coTRY(semaphore->Init(c), "Failed to init the semaphore: " << *semaphore);
	coASSERT(!imageAvailableSemaphore);
	coSwap(imageAvailableSemaphore, semaphore);
	return true;
}

coResult coVulkanSwapChain::InitFramebuffers()
{
	coTRY(renderPass, nullptr);
	coReserve(frameBuffers, images.count);

	for (coRenderImageView* imageView : imageViews)
	{
		coASSERT(imageView);
		const coRenderImage* image = imageView->GetImage();
		coTRY(image, nullptr);
		coRenderFramebuffer* fb = new coVulkanFramebuffer();
		coDEFER() { delete fb; };
		coRenderFramebuffer::InitConfig c;
		c.device = device;
		c.pass = renderPass;
		const coInt32x3& size = image->GetSize();
		coASSERT(size.z == 1);
		c.size = coInt32x2(size.x, size.y);
		coTODO("Because of weird Visual studio behavior in release.");
		coDynamicArray<coRenderImageView*> views;
		coPushBack(views, imageView);
		c.imageViews = imageViews;
		coTRY(fb->Init(c), "Failed to init framebuffer.");
		coPushBack(frameBuffers, fb);
		fb = nullptr;
	}

	return true;
}

coResult coVulkanSwapChain::InitPass()
{
	coVulkanPass* pass = new coVulkanPass();
	coDEFER() { delete pass; };
	coVulkanPass::InitConfig c;
	VkAttachmentDescription& att = c.attachmentDescription_vk;
	att.format = format_vk;
	att.samples = VK_SAMPLE_COUNT_1_BIT;
	att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	att.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	c.device = device;
	coTRY(pass->Init(c), "Failed to init render pass.");
	coSwap(renderPass, reinterpret_cast<coRenderPass*&>(pass));
	return true;
}
