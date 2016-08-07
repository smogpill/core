// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coPhysicalDevice_vk.h"
#include "render/vulkan/coSurface_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coDynamicArray_f.h"

coResult coGetNbPhysicalDevices_vk(coUint32& _nb, const VkInstance& _instance_vk)
{
	_nb = 0;
	coTRY_vk(vkEnumeratePhysicalDevices(_instance_vk, &_nb, nullptr), "Failed to count physical devices.");
	return true;
}

coResult coGetPhysicalDevices_vk(coDynamicArray<VkPhysicalDevice>& _out, const VkInstance& _instance_vk)
{
	coUint32 nb;
	coTRY(coGetNbPhysicalDevices_vk(nb, _instance_vk), nullptr);
	coResize(_out, nb);
	coTRY_vk(vkEnumeratePhysicalDevices(_instance_vk, &nb, _out.data), "Failed to retrieve the available physical devices");
	return true;
}

coResult coGetPhysicalDevices(coDynamicArray<coPhysicalDevice_vk*>& _out, const VkInstance& _instance_vk)
{
	coClear(_out);
	coDynamicArray<VkPhysicalDevice> supportedPhysicalDevices;
	coTRY(coGetPhysicalDevices_vk(supportedPhysicalDevices, _instance_vk), "Failed to retrieve the supported physical devices.");
	coReserve(_out, supportedPhysicalDevices.count);
	for (const VkPhysicalDevice& d : supportedPhysicalDevices)
	{
		coPhysicalDevice_vk* physicalDevice = new coPhysicalDevice_vk();
		coDEFER() { delete physicalDevice; };
		coPhysicalDevice_vk::InitConfig c;
		c.physicalDevice_vk = d;
		coTRY(physicalDevice->Init(c), "Failed to init the physical device: " << *physicalDevice);
		coPushBack(_out, physicalDevice);
		physicalDevice = nullptr;
	}
	return true;
}

coPhysicalDevice_vk::coPhysicalDevice_vk()
	: physicalDevice_vk(VK_NULL_HANDLE)
	, props_vk({})
{

}

coPhysicalDevice_vk::InitConfig::InitConfig()
	: physicalDevice_vk(VK_NULL_HANDLE)
{

}

coResult coPhysicalDevice_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	physicalDevice_vk = config.physicalDevice_vk;
	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	coTRY(InitQueueFamilyProperties(), "Failed to init queue family properties.");
	coTRY(InitSupportedExtensions(), "Failed to init supported extensions.");
	coTRY(InitProps(), "Failed to init device properties.");
	return true;
}

coResult coPhysicalDevice_vk::InitQueueFamilyProperties()
{
	coTRY(queueFamilyProperties.count == 0, nullptr);
	coUint32 nb = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_vk, &nb, nullptr);
	coResize(queueFamilyProperties, nb);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_vk, &queueFamilyProperties.count, queueFamilyProperties.data);
	coTRY(queueFamilyProperties.count == nb, nullptr);
	return true;
}

coResult coPhysicalDevice_vk::InitSupportedExtensions()
{
	coTRY(supportedExtensions.count == 0, nullptr);
	coUint32 nbExtensions = 0;
	coTRY_vk(vkEnumerateDeviceExtensionProperties(physicalDevice_vk, nullptr, &nbExtensions, nullptr), "Failed to get the device extensions count.");
	coResize(supportedExtensions, nbExtensions);
	coTRY_vk(vkEnumerateDeviceExtensionProperties(physicalDevice_vk, nullptr, &supportedExtensions.count, supportedExtensions.data), "Failed to get the device extensions.");
	coTRY(supportedExtensions.count == nbExtensions, nullptr);
	return true;
}

coResult coPhysicalDevice_vk::InitProps()
{
	vkGetPhysicalDeviceProperties(physicalDevice_vk, &props_vk);
	return true;
}

coBool coPhysicalDevice_vk::SupportsSurface(coUint queueFamilyIndex, const coSurface_vk& _surface_vk) const
{
	coASSERT(physicalDevice_vk != VK_NULL_HANDLE);
	VkBool32 supported_vk = VK_FALSE;
	coTRY_vk(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice_vk, queueFamilyIndex, _surface_vk.GetVkSurfaceKHR(), &supported_vk), "vkGetPhysicalDeviceSurfaceSupportKHR failed.");
	return supported_vk == VK_TRUE;
}

coBool coPhysicalDevice_vk::IsExtensionSupported(const coConstString& _extension) const
{
	for (const VkExtensionProperties& e : supportedExtensions)
	{
		if (_extension == e.extensionName)
			return true;
	}
	return false;
}
