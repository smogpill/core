// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "container/array/coDynamicArray_f.h"

coResult coGetNbPhysicalDevices_vk(coUint32& _nb, const VkInstance& _instance_vk)
{
	_nb = 0;
	coVULKAN_TRY(vkEnumeratePhysicalDevices(_instance_vk, &_nb, nullptr), "Failed to count physical devices.");
	return true;
}

coResult coGetPhysicalDevices_vk(coDynamicArray<VkPhysicalDevice>& _out, const VkInstance& _instance_vk)
{
	coUint32 nb;
	coTRY(coGetNbPhysicalDevices_vk(nb, _instance_vk), nullptr);
	coResize(_out, nb);
	coVULKAN_TRY(vkEnumeratePhysicalDevices(_instance_vk, &nb, _out.data), "Failed to retrieve the available physical devices");
	return true;
}

coResult coGetPhysicalDevices(coDynamicArray<coVulkanPhysicalDevice*>& _out, const VkInstance& _instance_vk)
{
	coClear(_out);
	coDynamicArray<VkPhysicalDevice> supportedPhysicalDevices;
	coTRY(coGetPhysicalDevices_vk(supportedPhysicalDevices, _instance_vk), "Failed to retrieve the supported physical devices.");
	coReserve(_out, supportedPhysicalDevices.count);
	for (const VkPhysicalDevice& d : supportedPhysicalDevices)
	{
		coVulkanPhysicalDevice* physicalDevice = new coVulkanPhysicalDevice();
		coDEFER() { delete physicalDevice; };
		coVulkanPhysicalDevice::InitConfig c;
		c.physicalDevice_vk = d;
		coTRY(physicalDevice->Init(c), "Failed to init the physical device: " << *physicalDevice);
		coPushBack(_out, physicalDevice);
		physicalDevice = nullptr;
	}
	return true;
}

coVulkanPhysicalDevice::coVulkanPhysicalDevice()
	: physicalDevice_vk(VK_NULL_HANDLE)
	, props_vk({})
{

}

coVulkanPhysicalDevice::InitConfig::InitConfig()
	: physicalDevice_vk(VK_NULL_HANDLE)
{

}

coResult coVulkanPhysicalDevice::OnInit(const coObject::InitConfig& _config)
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

coResult coVulkanPhysicalDevice::InitQueueFamilyProperties()
{
	coTRY(queueFamilyProperties.count == 0, nullptr);
	coUint32 nb = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_vk, &nb, nullptr);
	coResize(queueFamilyProperties, nb);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_vk, &queueFamilyProperties.count, queueFamilyProperties.data);
	coTRY(queueFamilyProperties.count == nb, nullptr);
	return true;
}

coResult coVulkanPhysicalDevice::InitSupportedExtensions()
{
	coTRY(supportedExtensions.count == 0, nullptr);
	coUint32 nbExtensions = 0;
	coVULKAN_TRY(vkEnumerateDeviceExtensionProperties(physicalDevice_vk, nullptr, &nbExtensions, nullptr), "Failed to get the device extensions count.");
	coResize(supportedExtensions, nbExtensions);
	coVULKAN_TRY(vkEnumerateDeviceExtensionProperties(physicalDevice_vk, nullptr, &supportedExtensions.count, supportedExtensions.data), "Failed to get the device extensions.");
	coTRY(supportedExtensions.count == nbExtensions, nullptr);
	return true;
}

coResult coVulkanPhysicalDevice::InitProps()
{
	vkGetPhysicalDeviceProperties(physicalDevice_vk, &props_vk);
	return true;
}

coBool coVulkanPhysicalDevice::SupportsSurface(coUint queueFamilyIndex, const coVulkanSurface& _surface_vk) const
{
	coASSERT(physicalDevice_vk != VK_NULL_HANDLE);
	VkBool32 supported_vk = VK_FALSE;
	coVULKAN_TRY(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice_vk, queueFamilyIndex, _surface_vk.GetVkSurfaceKHR(), &supported_vk), "vkGetPhysicalDeviceSurfaceSupportKHR failed.");
	return supported_vk == VK_TRUE;
}

coBool coVulkanPhysicalDevice::IsExtensionSupported(const coConstString& _extension) const
{
	for (const VkExtensionProperties& e : supportedExtensions)
	{
		if (_extension == e.extensionName)
			return true;
	}
	return false;
}
