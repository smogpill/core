// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult_f.h"

class coRendererInfo_vk
{
public:
	coRendererInfo_vk()
		: instance_vk(VK_NULL_HANDLE)
	{
	}
	VkInstance instance_vk;
	VkAllocationCallbacks allocator_vk;
};

coResult coGetNbExtensions(coUint32& nbExtensions)
{
	nbExtensions = 0;
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &nbExtensions, nullptr), "Failed to get the extension count.");
	return true;
}

coResult coGetExtensions(coDynamicArray<VkExtensionProperties>& _extensions)
{
	coUint32 nbExtensions = 0;
	coTRY(coGetNbExtensions(nbExtensions), nullptr);
	coResize(_extensions, nbExtensions);
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &nbExtensions, _extensions.data), "Failed to get the extension properties");
	return true;
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
	{
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
	}

	// Instance
	coASSERT(info_vk->instance_vk == VK_NULL_HANDLE);
	coTRY_vk(vkCreateInstance(&createInfo, nullptr, &info_vk->instance_vk), "Failed to create Vulkan instance.");
	return true;
}
