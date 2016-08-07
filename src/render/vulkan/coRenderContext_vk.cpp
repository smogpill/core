// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coRenderContext_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coMessageHandler_vk.h"
#include "render/vulkan/coExtensionManager_vk.h"
#include "render/vulkan/coLayerManager_vk.h"
#include "render/vulkan/coPhysicalDevice_vk.h"
#include "render/vulkan/coLogicalDevice_vk.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coDynamicArray_f.h"

coRenderContext_vk::coRenderContext_vk()
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

coRenderContext_vk::~coRenderContext_vk()
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

coResult coRenderContext_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	coTRY(InitLayers(), "Failed to init layers.");
	coTRY(InitExtensions(), "Failed to init extensions.");
	coTRY(InitInstance(), "Failed to init the Vulkan instance.");
	coTRY(InitMessageHandler(), "Failed to init the message handler.");
	coTRY(InitDevices(), "Failed to init the devices");

	return true;
}

coResult coRenderContext_vk::InitLayers()
{
	coLayerManager_vk* manager = new coLayerManager_vk();
	coDEFER() { delete manager; };
	coLayerManager_vk::InitConfig c;
	coTRY(manager->Init(c), "Failed to init the layer manager.");
	if (enableDebug)
	{
		coCHECK(manager->AddRequested("VK_LAYER_LUNARG_standard_validation"), "Failed to add layer, ignored.");
	}
	coSwap(layerManager_vk, manager);
	return true;
}

coResult coRenderContext_vk::InitExtensions()
{
	coExtensionManager_vk* manager = new coExtensionManager_vk();
	coDEFER() { delete manager; };
	coExtensionManager_vk::InitConfig c;
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

coResult coRenderContext_vk::InitInstance()
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
	coTRY_vk(vkCreateInstance(&createInfo, nullptr, &instance_vk), "Failed to create Vulkan instance.");

	return true;
}

coResult coRenderContext_vk::InitMessageHandler()
{
	if (enableDebug)
	{
		coMessageHandler_vk* p = new coMessageHandler_vk();
		coDEFER() { delete p; };
		coMessageHandler_vk::InitConfig c;
		c.instance_vk = &instance_vk;
		coTRY(p->Init(c), "Failed to init the message handler.");
		coSwap(messageHandler_vk, p);
	}
	return true;
}

coResult coRenderContext_vk::InitDevices()
{
	coTRY(instance_vk, nullptr);
	coTRY(logicalDevices.count == 0, nullptr);

	coTRY(coGetPhysicalDevices(physicalDevices, instance_vk), "Failed to retrieve the supported physical devices.");
	coTRY(physicalDevices.count > 0, "Failed to find any supported physical device.");

	for (coPhysicalDevice_vk* physicalDevice : physicalDevices)
	{
		coTRY(physicalDevice, nullptr);

		if (!physicalDevice->IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		{
			coWARN("The physical device does not support swap chain, ignoring it: " << *physicalDevice);
			continue;
		}

		coLogicalDevice_vk* logicalDevice = new coLogicalDevice_vk();
		coDEFER() { delete logicalDevice; };
		coLogicalDevice_vk::InitConfig c;
		c.physicalDevice_vk = physicalDevice;
		c.layerManager_vk = layerManager_vk;
		coTRY(logicalDevice->Init(c), "Failed to init the device: " << *logicalDevice);
		
		coTRY(logicalDevice->AddRequestedExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME), nullptr);
		coUint32 score = 0;
		coTRY(logicalDevice->GetScore(score), "Failed to get the device score");
		if (score > 0)
		{
			coPushBack(logicalDevices, logicalDevice);
			logicalDevice = nullptr;
		}
	}

	coTRY(logicalDevices.count > 0, "Failed to find any acceptable device.");
	return true;
}
