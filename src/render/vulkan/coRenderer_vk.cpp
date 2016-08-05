// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coMessageHandler_vk.h"
#include "render/vulkan/coExtensionManager_vk.h"
#include "render/vulkan/coLayerManager_vk.h"
#include "render/vulkan/coRendererInfo_vk.h"
#include "render/vulkan/coDevice_vk.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coArray_f.h"

coResult coInitLayers_vk(coRendererInfo_vk& _info_vk)
{
	coLayerManager_vk* manager = new coLayerManager_vk();
	coDEFER() { delete manager; };
	coLayerManager_vk::InitConfig c;
	coTRY(manager->Init(c), "Failed to init the layer manager.");
	if (_info_vk.enableDebug)
	{
		coCHECK(manager->AddRequested("VK_LAYER_LUNARG_standard_validation"), "Failed to add layer, ignored.");
	}
	coSwap(_info_vk.layerManager_vk, manager);
	return true;
}

coResult coInitExtensions_vk(coRendererInfo_vk& _info_vk)
{
	coExtensionManager_vk* manager = new coExtensionManager_vk();
	coDEFER() { delete manager; };
	coExtensionManager_vk::InitConfig c;
	coTRY(manager->Init(c), "Failed to init the extension manager.");
	if (_info_vk.enableDebug)
	{
		coCHECK(manager->AddRequested("VK_EXT_debug_report"), "Failed to add extension, ignored.");
	}
	coSwap(_info_vk.extensionManager_vk, manager);
	return true;
}

coResult coInitInstance_vk(coRendererInfo_vk& _info_vk)
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
		coTRY(_info_vk.layerManager_vk->GetAllRequested(requestedLayers), nullptr);
		createInfo.enabledLayerCount = requestedLayers.count;
		createInfo.ppEnabledLayerNames = requestedLayers.data;

		// Set extensions
		coTRY(_info_vk.extensionManager_vk->GetAllRequested(requestedExtensions), nullptr);
		createInfo.enabledExtensionCount = requestedExtensions.count;
		createInfo.ppEnabledExtensionNames = requestedExtensions.data;
	}

	// Instance
	coASSERT(_info_vk.instance_vk == VK_NULL_HANDLE);
	coTRY_vk(vkCreateInstance(&createInfo, nullptr, &_info_vk.instance_vk), "Failed to create Vulkan instance.");

	return true;
}

coResult coInitMessageHandler_vk(coRendererInfo_vk& _info_vk)
{
	if (_info_vk.enableDebug)
	{
		coMessageHandler_vk* p = new coMessageHandler_vk();
		coDEFER() { delete p; };
		coMessageHandler_vk::InitConfig c;
		c.instance_vk = &_info_vk.instance_vk;
		coTRY(p->Init(c), "Failed to init the message handler.");
		coSwap(_info_vk.messageHandler_vk, p);
	}
	return true;
}

coResult coInitDevices_vk(coRendererInfo_vk& _info_vk)
{
	coTRY(_info_vk.instance_vk, nullptr);
	coTRY(_info_vk.devices.count == 0, nullptr);

	coDynamicArray<VkPhysicalDevice> availableDevices;
	coTRY(coGetPhysicalDevices_vk(availableDevices, _info_vk.instance_vk), "Failed to retrieve available physical devices");
	coTRY(availableDevices.count > 0, "Failed to find any available physical device.");

	for (const VkPhysicalDevice& physicalDevice : availableDevices)
	{
		coDevice_vk* device = new coDevice_vk();
		coDEFER() { delete device; };
		coDevice_vk::InitConfig c;
		c.physicalDevice_vk = physicalDevice;
		c.layerManager_vk = _info_vk.layerManager_vk;
		coTRY(device->Init(c), "Failed to init the device: " << *device);
		coUint32 score = 0;
		coTRY(device->GetScore(score), "Failed to get the device score");
		if (score > 0)
		{
			coPushBack(_info_vk.devices, device);
			device = nullptr;
		}
	}

	coTRY(_info_vk.devices.count > 0, "Failed to find any acceptable device.");
	return true;
}

void coRenderer::OnImplConstruct()
{
	impl = new coRendererInfo_vk();
}

void coRenderer::OnImplDestruct()
{
	coRendererInfo_vk* info_vk = static_cast<coRendererInfo_vk*>(impl);
	for (coDevice_vk* device : info_vk->devices)
		delete device;
	coClear(info_vk->devices);
	delete info_vk->messageHandler_vk;
	info_vk->messageHandler_vk = nullptr;
	vkDestroyInstance(info_vk->instance_vk, nullptr);
	delete info_vk->layerManager_vk;
	delete info_vk;
}

coResult coRenderer::OnImplInit(const InitConfig& /*_config*/)
{
	coRendererInfo_vk* info_vk = static_cast<coRendererInfo_vk*>(impl);

	coTRY(coInitLayers_vk(*info_vk), "Failed to init layers.");
	coTRY(coInitExtensions_vk(*info_vk), "Failed to init extensions.");
	coTRY(coInitInstance_vk(*info_vk), "Failed to init the Vulkan instance.");
	coTRY(coInitMessageHandler_vk(*info_vk), "Failed to init the message handler.");
	coTRY(coInitDevices_vk(*info_vk), "Failed to init the devices");
	return true;
}
