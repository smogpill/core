// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanRenderer.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanMessageHandler.h"
#include "render_vulkan/coVulkanExtensionManager.h"
#include "render_vulkan/coVulkanLayerManager.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coDynamicArray_f.h"

coVulkanRenderer::coVulkanRenderer()
	: instance_vk(VK_NULL_HANDLE)
	, enableDebug(false)
	, messageHandler_vk(nullptr)
	, layerManager_vk(nullptr)
	, extensionManager_vk(nullptr)
{
#ifdef coDEBUG
	enableDebug = true;
#endif
}

coVulkanRenderer::~coVulkanRenderer()
{
	for (auto p : logicalDevices)
		delete p;
	coClear(logicalDevices);
	for (auto p : physicalDevices)
		delete p;
	coClear(physicalDevices);
	delete messageHandler_vk;
	vkDestroyInstance(instance_vk, nullptr);
	delete extensionManager_vk;
	delete layerManager_vk;
}

coResult coVulkanRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	//const InitConfig& config = static_cast<const InitConfig&>(_config);
	coTRY(InitLayers(), "Failed to init layers.");
	coTRY(InitExtensions(), "Failed to init extensions.");
	coTRY(InitInstance(), "Failed to init the Vulkan instance.");
	coTRY(InitMessageHandler(), "Failed to init the message handler.");
	coTRY(InitDevices(), "Failed to init the devices");
	return true;
}

coResult coVulkanRenderer::InitLayers()
{
	coVulkanLayerManager* manager = new coVulkanLayerManager();
	coDEFER() { delete manager; };
	coVulkanLayerManager::InitConfig c;
	coTRY(manager->Init(c), "Failed to init the layer manager.");
	if (enableDebug)
	{
		coCHECK(manager->AddRequested("VK_LAYER_LUNARG_standard_validation"), "Failed to add layer, ignored.");
	}
	coSwap(layerManager_vk, manager);
	return true;
}

coResult coVulkanRenderer::InitExtensions()
{
	coVulkanExtensionManager* manager = new coVulkanExtensionManager();
	coDEFER() { delete manager; };
	coVulkanExtensionManager::InitConfig c;
	coTRY(manager->Init(c), "Failed to init the extension manager.");
	coTRY(manager->AddRequested(VK_KHR_SURFACE_EXTENSION_NAME), nullptr);
#ifdef coMSWINDOWS
	coTRY(manager->AddRequested(VK_KHR_WIN32_SURFACE_EXTENSION_NAME), nullptr);
#else
#	error Platform not yet supported for surfaces.
#endif
	if (enableDebug)
	{
		coCHECK(manager->AddRequested(VK_EXT_DEBUG_REPORT_EXTENSION_NAME), "Failed to add extension, ignored.");
	}
	coSwap(extensionManager_vk, manager);
	return true;
}

coResult coVulkanRenderer::InitInstance()
{
	// Application Info
	VkApplicationInfo appInfo = {};
	{
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Core";
		appInfo.applicationVersion = VK_MAKE_VERSION(coVERSION_MAJOR, coVERSION_MINOR, coVERSION_BUILD);
		appInfo.pEngineName = "Core";
		appInfo.engineVersion = VK_MAKE_VERSION(coVERSION_MAJOR, coVERSION_MINOR, coVERSION_BUILD);
		appInfo.apiVersion = VK_API_VERSION_1_0;
	}

	// Instance create info
	VkInstanceCreateInfo createInfo = {};
	coDynamicArray<const coChar*> requestedLayers;
	coDynamicArray<const coChar*> requestedExtensions;
	{
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Set layers
		coTRY(layerManager_vk->GetAllRequested(requestedLayers), nullptr);
		createInfo.enabledLayerCount = requestedLayers.count;
		createInfo.ppEnabledLayerNames = requestedLayers.data;

		// Set extensions
		coTRY(extensionManager_vk->GetAllRequested(requestedExtensions), nullptr);
		createInfo.enabledExtensionCount = requestedExtensions.count;
		createInfo.ppEnabledExtensionNames = requestedExtensions.data;
	}

	// Instance
	coASSERT(instance_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateInstance(&createInfo, nullptr, &instance_vk), "Failed to create Vulkan instance.");

	return true;
}

coResult coVulkanRenderer::InitMessageHandler()
{
	if (enableDebug)
	{
		coVulkanMessageHandler* p = new coVulkanMessageHandler();
		coDEFER() { delete p; };
		coVulkanMessageHandler::InitConfig c;
		c.instance_vk = &instance_vk;
		coTRY(p->Init(c), "Failed to init the message handler.");
		coSwap(messageHandler_vk, p);
	}
	return true;
}

coResult coVulkanRenderer::InitDevices()
{
	coTRY(instance_vk, nullptr);
	coTRY(logicalDevices.count == 0, nullptr);

	coTRY(coGetPhysicalDevices(physicalDevices, instance_vk), "Failed to retrieve the supported physical devices.");
	coTRY(physicalDevices.count > 0, "Failed to find any supported physical device.");

	for (coVulkanPhysicalDevice* physicalDevice : physicalDevices)
	{
		coTRY(physicalDevice, nullptr);

		if (!physicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		{
			coWARN("The physical device does not support swap chain, ignoring it: " << *physicalDevice);
			continue;
		}

		coVulkanLogicalDevice* logicalDevice = new coVulkanLogicalDevice();
		coDEFER() { delete logicalDevice; };
		coVulkanLogicalDevice::InitConfig c;
		c.physicalDevice_vk = physicalDevice;
		c.layerManager_vk = layerManager_vk;
		coTRY(logicalDevice->Init(c), "Failed to init the device: " << *logicalDevice);
		const coRenderDevice::DeviceType deviceType = logicalDevice->GetDeviceType();
		if (deviceType != coRenderDevice::discreteGpu && deviceType != coRenderDevice::integratedGpu)
		{
			continue;
		}
		coTRY(logicalDevice->AddRequestedExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME), nullptr);
		coPushBack(logicalDevices, logicalDevice);
		logicalDevice = nullptr;
	}

	coTRY(logicalDevices.count > 0, "Failed to find any acceptable device.");
	return true;
}
