// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanLayerManager.h"
#include "render_vulkan/coVulkanSurface.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coVulkanLogicalDevice::coVulkanLogicalDevice()
	: physicalDevice_vk(nullptr)
	, logicalDevice_vk(VK_NULL_HANDLE)
	//, queues_vk{ VK_NULL_HANDLE }
	, queueIds{ -1 }
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
// 	for (VkQueue& queue_vk : queues_vk)
// 		queue_vk = VK_NULL_HANDLE;
	vkDestroyDevice(logicalDevice_vk, nullptr);
	logicalDevice_vk = VK_NULL_HANDLE;
	Super::OnStop();
}

coResult coVulkanLogicalDevice::InitQueueFamilyIndices(coVulkanSurface* _surface_vk)
{
	for (auto& index : queueIds)
		index = -1;

	const coArray<VkQueueFamilyProperties>& queueFamilyProps = physicalDevice_vk->GetQueueFamilyProperties();
	for (coUint i = 0; i < queueFamilyProps.count; ++i)
	{
		const VkQueueFamilyProperties& prop = queueFamilyProps[i];
		if (prop.queueCount > 0)
		{
			if (queueIds[QueueType::graphics] == -1 && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queueIds[QueueType::graphics] = i;
			}

			if (_surface_vk)
			{
				if (queueIds[QueueType::present] == -1)
				{
					if (physicalDevice_vk->SupportsSurface(i, *_surface_vk))
					{
						queueIds[QueueType::present] = i;
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
	coTRY(queueIds[QueueType::graphics] >= 0, nullptr);
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = GetFamilyIndex(QueueType::graphics);
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

coInt32 coVulkanLogicalDevice::GetFamilyIndex(QueueType _type) const
{
	return coCastWithOverflowCheck<coInt32>(queueIds[_type]);
}

coResult coVulkanLogicalDevice::InitQueues()
{
// 
// 	// Get unique queue families
// 	coDynamicArray<coInt32> uniqueQueueFamilies;
// 	for (const auto& index : queueFamilyIndices)
// 	{
// 		if (index != -1 && !coContains(uniqueQueueFamilies, index))
// 			coPushBack(uniqueQueueFamilies, index);
// 	}
// 	
// 	// Get queues
// 	for (coInt32 index : uniqueQueueFamilies)
// 	{
// 		coTRY(index >= 0, nullptr);
// 		VkQueue queue_vk;
// 		vkGetDeviceQueue(logicalDevice_vk, index, 0, &queue_vk);
// 		coPushBack(queues_vk, queue_vk);
// 		if (index == graphicsFamilyIndex)
// 			graphicsQueue_vk = queue_vk;
// 		if (index == presentFamilyIndex)
// 			presentQueue_vk = queue_vk;
// 	}
	
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

	if (queueIds[QueueType::graphics] < 0)
		return true;

	_out = 1;

	if (queueIds[QueueType::present] < 0)
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

coResult coVulkanLogicalDevice::WaitForIdle()
{
	coVULKAN_TRY(vkDeviceWaitIdle(logicalDevice_vk), "Wait device for idle failed.");
	return true;
}
