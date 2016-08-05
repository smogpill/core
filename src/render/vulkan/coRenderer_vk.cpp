// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coMessageHandler_vk.h"
#include "render/vulkan/coExtensions_f_vk.h"
#include "render/vulkan/coLayers_f_vk.h"
#include "render/vulkan/coRendererInfo_vk.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coArray_f.h"

void coRenderer::OnImplConstruct()
{
	impl = new coRendererInfo_vk();
}

void coRenderer::OnImplDestruct()
{
	coRendererInfo_vk* info_vk = static_cast<coRendererInfo_vk*>(impl);
	delete info_vk->messageHandler_vk;
	info_vk->messageHandler_vk = nullptr;
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
			coTRY(coCheckIfExtensionsAreAvailable_vk(requestedExtensions), nullptr);
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

			coTRY(coCheckIfLayersAreAvailable_vk(requestedLayers), nullptr);
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