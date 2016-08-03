// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coMessageHandler_vk.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coArray_f.h"

class coRendererInfo_vk
{
public:
	coRendererInfo_vk()
		: instance_vk(VK_NULL_HANDLE)
		, enableDebug(false)
		, messageHandler_vk(nullptr)
	{
#ifdef coDEBUG
		enableDebug = true;
#endif
	}
	VkInstance instance_vk;
	VkAllocationCallbacks allocator_vk;
	coBool enableDebug;
	coMessageHandler_vk* messageHandler_vk;
};

static coResult coGetNbAvailableExtensions(coUint32& _nbExtensions)
{
	_nbExtensions = 0;
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &_nbExtensions, nullptr), "Failed to get the extension count.");
	return true;
}

static coResult coGetAvailableExtensions(coDynamicArray<VkExtensionProperties>& _extensions)
{
	coUint32 nbExtensions = 0;
	coTRY(coGetNbAvailableExtensions(nbExtensions), nullptr);
	coResize(_extensions, nbExtensions);
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &nbExtensions, _extensions.data), "Failed to get the extension properties");
	return true;
}

static coResult coCheckIfExtensionsAreAvailable(const coArray<const coChar*>& _requestedExtensions)
{
	coDynamicArray<VkExtensionProperties> availableExtensions;
	coTRY(coGetAvailableExtensions(availableExtensions), "Failed to get available extensions");

	coBool missingExtensions = false;
	for (const coConstString& requested : _requestedExtensions)
	{
		coBool found = false;
		for (const VkExtensionProperties& available : availableExtensions)
		{
			if (requested == available.extensionName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			coWARN("Vulkan extension not available: " << requested);
			missingExtensions = true;
		}
	}

	return !missingExtensions;
}

static coResult coGetNbAvailableLayers(coUint32& _nbLayers)
{
	_nbLayers = 0;
	coTRY_vk(vkEnumerateInstanceLayerProperties(&_nbLayers, nullptr), "Failed to get the layer count.");
	return true;
}

static coResult coGetAvailableLayers(coDynamicArray<VkLayerProperties>& _validationLayers)
{
	coUint32 nbLayers;
	coTRY(coGetNbAvailableLayers(nbLayers), nullptr);
	coResize(_validationLayers, nbLayers);
	coTRY_vk(vkEnumerateInstanceLayerProperties(&nbLayers, _validationLayers.data), "Failed to get available layers.");
	return true;
}

static coResult coCheckIfLayersAreAvailable(const coArray<const coChar*>& _requestedLayers)
{
	coDynamicArray<VkLayerProperties> availableLayers;
	coTRY(coGetAvailableLayers(availableLayers), "Failed to get available layers");

	coBool missingLayers = false;
	for (const coConstString& requested : _requestedLayers)
	{
		coBool found = false;
		for (const VkLayerProperties& available : availableLayers)
		{
			if (requested == available.layerName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			coWARN("Vulkan layer not available: "<<requested);
			missingLayers = true;
		}
	}

	return !missingLayers;
}

void coRenderer::OnImplConstruct()
{
	impl = new coRendererInfo_vk();
}

void coRenderer::OnImplDestruct()
{
	coRendererInfo_vk* info_vk = static_cast<coRendererInfo_vk*>(impl);
	vkDestroyInstance(info_vk->instance_vk, nullptr);
	delete info_vk;
}

coResult coRenderer::OnImplInit(const InitConfig& /*_config*/)
{
	coRendererInfo_vk* info_vk = static_cast<coRendererInfo_vk*>(impl);

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
		// Set extensions
		{
			if (info_vk->enableDebug)
			{
				coPushBack(requestedExtensions, "VK_EXT_debug_report");
			}
			coTRY(coCheckIfExtensionsAreAvailable(requestedExtensions), nullptr);
			createInfo.enabledExtensionCount = requestedExtensions.count;
			createInfo.ppEnabledExtensionNames = requestedExtensions.data;
		}

		// Set layers
		if (info_vk->enableDebug)
		{
			if (info_vk->enableDebug)
			{
				coPushBack(requestedLayers, "VK_LAYER_LUNARG_standard_validation");
			}

			coTRY(coCheckIfLayersAreAvailable(requestedLayers), nullptr);
			createInfo.enabledLayerCount = requestedLayers.count;
			createInfo.ppEnabledLayerNames = requestedLayers.data;
		}

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
	}

	// Instance
	coASSERT(info_vk->instance_vk == VK_NULL_HANDLE);
	coTRY_vk(vkCreateInstance(&createInfo, nullptr, &info_vk->instance_vk), "Failed to create Vulkan instance.");

	if (info_vk->enableDebug)
	{
		coMessageHandler_vk* p = new coMessageHandler_vk();
		coDEFER() { delete p; };
		coMessageHandler_vk::InitConfig c;
		c.instance_vk = &info_vk->instance_vk;
		coTRY(p->Init(c), "Failed to init the message handler.");
		coSwap(info_vk->messageHandler_vk, p);
	}
	return true;
}
