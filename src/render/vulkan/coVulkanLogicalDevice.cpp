// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coVulkanLogicalDevice.h"
#include "render/vulkan/coVulkanPhysicalDevice.h"
#include "render/vulkan/coVulkanResult_f.h"
#include "render/vulkan/coVulkanLayerManager.h"
#include "render/vulkan/coVulkanSurface.h"
#include "lang/result/coResult_f.h"

coVulkanLogicalDevice::coVulkanLogicalDevice()
	: physicalDevice_vk(nullptr)
	, logicalDevice_vk(VK_NULL_HANDLE)
	, graphicsQueue_vk(VK_NULL_HANDLE)
	, presentQueue_vk(VK_NULL_HANDLE)
	, graphicsFamilyIndex(-1)
	, presentFamilyIndex(-1)
	, layerManager_vk(nullptr)
{

}

coVulkanLogicalDevice::~coVulkanLogicalDevice()
{
	vkDestroyDevice(logicalDevice_vk, nullptr);
	for (auto p : requestedExtensions)
		delete p;
}

coVulkanLogicalDevice::InitConfig::InitConfig()
	: physicalDevice_vk(VK_NULL_HANDLE)
	, layerManager_vk(nullptr)
	, surface_vk(nullptr)
{

}

coResult coVulkanLogicalDevice::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coASSERT(physicalDevice_vk == VK_NULL_HANDLE);
	coTRY(config.physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	physicalDevice_vk = config.physicalDevice_vk;
	layerManager_vk = config.layerManager_vk;
	coTRY(InitQueueFamilyIndices(config.surface_vk), "Failed to init the queue family index");
	return true;
}

coResult coVulkanLogicalDevice::OnStart()
{
	coTRY(Super::OnStart(), nullptr);

	coTRY(InitLogicalDevice(), "Failed to init the logical device.");
	coTRY(InitQueues(), "Failed to init the queue.");
	return true;
}

void coVulkanLogicalDevice::OnStop()
{
	graphicsQueue_vk = VK_NULL_HANDLE;
	presentQueue_vk = VK_NULL_HANDLE;
	coClear(queues);
	vkDestroyDevice(logicalDevice_vk, nullptr);
	logicalDevice_vk = VK_NULL_HANDLE;
	Super::OnStop();
}

coResult coVulkanLogicalDevice::InitQueueFamilyIndices(coVulkanSurface* _surface_vk)
{
	graphicsFamilyIndex = -1;
	presentFamilyIndex = -1;

	const coArray<VkQueueFamilyProperties>& queueFamilyProps = physicalDevice_vk->GetQueueFamilyProperties();
	for (coUint i = 0; i < queueFamilyProps.count; ++i)
	{
		const VkQueueFamilyProperties& prop = queueFamilyProps[i];
		if (prop.queueCount > 0)
		{
			if (graphicsFamilyIndex == -1 && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsFamilyIndex = i;
			}

			if (_surface_vk)
			{
				if (presentFamilyIndex == -1)
				{
					if (physicalDevice_vk->SupportsSurface(i, *_surface_vk))
					{
						presentFamilyIndex = i;
					}
				}
			}
			break;
		}
	}
	return true;
}

coResult coVulkanLogicalDevice::InitLogicalDevice()
{
	coTRY(graphicsFamilyIndex >= 0, nullptr);
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
	queueCreateInfo.queueCount = 1;
	coFloat priority = 1.0f;
	queueCreateInfo.pQueuePriorities = &priority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	coDynamicArray<const coChar*> layers;
	if (layerManager_vk)
	{
		layerManager_vk->GetAllRequested(layers);
	}

	coDynamicArray<const coChar*> extensions;
	coTRY(GetAllRequestedExtensions(extensions), nullptr);

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = extensions.count;
	createInfo.ppEnabledExtensionNames = extensions.data;
	createInfo.enabledLayerCount = layers.count;
	createInfo.ppEnabledLayerNames = layers.data;

	coTRY(logicalDevice_vk == VK_NULL_HANDLE, nullptr);
	coTRY(vkCreateDevice(physicalDevice_vk->GetVkPhysicalDevice(), &createInfo, nullptr, &logicalDevice_vk), "Failed to create device");

	return true;
}

coResult coVulkanLogicalDevice::InitQueues()
{
	coTRY(queues.count == 0, nullptr);

	// Get unique queue families
	coDynamicArray<coInt32> uniqueQueueFamilies;
	if (graphicsFamilyIndex != -1 && !coContains(uniqueQueueFamilies, graphicsFamilyIndex))
		coPushBack(uniqueQueueFamilies, graphicsFamilyIndex);
	if (presentFamilyIndex != -1 && !coContains(uniqueQueueFamilies, presentFamilyIndex))
		coPushBack(uniqueQueueFamilies, presentFamilyIndex);

	// Get queues
	for (coInt32 index : uniqueQueueFamilies)
	{
		coTRY(index >= 0, nullptr);
		VkQueue queue_vk;
		vkGetDeviceQueue(logicalDevice_vk, index, 0, &queue_vk);
		coPushBack(queues, queue_vk);
		if (index == graphicsFamilyIndex)
			graphicsQueue_vk = queue_vk;
		if (index == presentFamilyIndex)
			presentQueue_vk = queue_vk;
	}
	
	return true;
}

coResult coVulkanLogicalDevice::GetScore(coUint32& _out) const
{
	_out = 0;

	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);

	const VkPhysicalDeviceProperties& deviceProperties = physicalDevice_vk->GetProperties();
	if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		return true;
	}

	if (graphicsFamilyIndex < 0)
		return true;

	_out = 1;

	if (presentFamilyIndex < 0)
		return true;

	++_out;

	return true;
}

coResult coVulkanLogicalDevice::AddRequestedExtension(const coConstString& _extension)
{
	coTRY(physicalDevice_vk->IsExtensionSupported(_extension), "Device extension not supported: " << _extension);
	coTRY(!IsExtensionRequested(_extension), "Device extension already requested: " << _extension);
	coDynamicString* extension = new coDynamicString(_extension);
	coNullTerminate(*extension);
	coPushBack(requestedExtensions, extension);
	return true;
}

coResult coVulkanLogicalDevice::IsExtensionRequested(const coConstString& _extension) const
{
	for (const coDynamicString* s : requestedExtensions)
	{
		coASSERT(s);
		if (*s == _extension)
			return true;
	}
	return false;
}

coResult coVulkanLogicalDevice::GetAllRequestedExtensions(coDynamicArray<const coChar*>& _extensions) const
{
	coClear(_extensions);
	for (const coDynamicString* s : requestedExtensions)
	{
		coASSERT(coIsNullTerminated(*s));
		coPushBack(_extensions, s->data);
	}
	return true;
}

coResult coVulkanLogicalDevice::WaitIdle()
{
	coVULKAN_TRY(vkDeviceWaitIdle(logicalDevice_vk), "Wait device for idle failed.");
	return true;
}
