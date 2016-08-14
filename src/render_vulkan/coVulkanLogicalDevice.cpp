// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanLayerManager.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coVulkanLogicalDevice::coVulkanLogicalDevice()
	: vulkanPhysicalDevice(nullptr)
	, logicalDevice_vk(VK_NULL_HANDLE)
	//, queues_vk{ VK_NULL_HANDLE }
	, queueFamilyIndices{ -1 }
	, vulkanLayerManager(nullptr)
{

}

coVulkanLogicalDevice::~coVulkanLogicalDevice()
{
	coCHECK(WaitForIdle(), nullptr);
	vkDestroyDevice(logicalDevice_vk, nullptr);
	for (auto p : requestedExtensions)
		delete p;
}

coVulkanLogicalDevice::InitConfig::InitConfig()
	: physicalDevice_vk(VK_NULL_HANDLE)
	, layerManager_vk(nullptr)
{

}

coResult coVulkanLogicalDevice::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coASSERT(vulkanPhysicalDevice == VK_NULL_HANDLE);
	coTRY(config.physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	vulkanPhysicalDevice = config.physicalDevice_vk;
	vulkanLayerManager = config.layerManager_vk;
	coTRY(InitQueueFamilyIndices(), "Failed to init the queue family index");
	return true;
}

coResult coVulkanLogicalDevice::OnStart()
{
	coTRY(Super::OnStart(), nullptr);

	coTRY(InitLogicalDevice(), "Failed to init the logical device.");
	coTRY(InitQueues(), "Failed to init queues.");
	return true;
}

void coVulkanLogicalDevice::OnStop()
{
	coCHECK(WaitForIdle(), nullptr);
	for (VkQueue& queue_vk : queues_vk)
		queue_vk = VK_NULL_HANDLE;
	vkDestroyDevice(logicalDevice_vk, nullptr);
	logicalDevice_vk = VK_NULL_HANDLE;
	Super::OnStop();
}

coResult coVulkanLogicalDevice::InitQueueFamilyIndices()
{
	for (auto& index : queueFamilyIndices)
		index = -1;

	const coArray<VkQueueFamilyProperties>& queueFamilyProps = vulkanPhysicalDevice->GetQueueFamilyProperties();
	for (coUint i = 0; i < queueFamilyProps.count; ++i)
	{
		const VkQueueFamilyProperties& prop = queueFamilyProps[i];
		if (prop.queueCount > 0)
		{
			if (queueFamilyIndices[QueueType::graphics] == -1 && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queueFamilyIndices[QueueType::graphics] = i;
			}

			if (queueFamilyIndices[QueueType::compute] == -1 && prop.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				queueFamilyIndices[QueueType::compute] = i;
			}

			if (queueFamilyIndices[QueueType::transfer] == -1 && prop.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				queueFamilyIndices[QueueType::transfer] = i;
			}
			break;
		}
	}
	return true;
}

coResult coVulkanLogicalDevice::InitQueues()
{
	static_assert(coARRAY_SIZE(queues_vk) == coARRAY_SIZE(queueFamilyIndices), "");
	for (coUint i = 0; i < coARRAY_SIZE(queueFamilyIndices); ++i)
	{
		const coInt32 queueFamilyIndex = queueFamilyIndices[i];
		if (queueFamilyIndex != -1)
		{
			const uint32_t queueFamilyIndex_vk = coCastWithOverflowCheck<uint32_t>(queueFamilyIndex);
			coTRY(GetVkQueue(queues_vk[i], queueFamilyIndex_vk, 0), nullptr);
		}
		else
		{
			queues_vk[i] = VK_NULL_HANDLE;
		}
	}
	return true;
}

coResult coVulkanLogicalDevice::InitLogicalDevice()
{
	coTRY(queueFamilyIndices[QueueType::graphics] >= 0, nullptr);
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndices[QueueType::graphics];
	queueCreateInfo.queueCount = 1;
	coFloat priority = 1.0f;
	queueCreateInfo.pQueuePriorities = &priority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	coDynamicArray<const coChar*> layers;
	if (vulkanLayerManager)
	{
		vulkanLayerManager->GetAllRequested(layers);
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
	coTRY(vkCreateDevice(vulkanPhysicalDevice->GetVkPhysicalDevice(), &createInfo, nullptr, &logicalDevice_vk), "Failed to create device");

	return true;
}

coResult coVulkanLogicalDevice::GetVkQueue(VkQueue& _out, coUint _queueFamilyIndex, coUint _index)
{
	_out = VK_NULL_HANDLE;
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	vkGetDeviceQueue(device_vk, _queueFamilyIndex, _index, &_out);
	return true;
}

coResult coVulkanLogicalDevice::AddRequestedExtension(const coConstString& _extension)
{
	coTRY(vulkanPhysicalDevice->IsExtensionSupported(_extension), "Device extension not supported: " << _extension);
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

auto coVulkanLogicalDevice::GetDeviceType() const -> DeviceType
{
	if (vulkanPhysicalDevice)
	{
		const VkPhysicalDeviceProperties& deviceProperties = vulkanPhysicalDevice->GetProperties();
		switch (deviceProperties.deviceType)
		{
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return DeviceType::integratedGpu;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return DeviceType::discreteGpu;
		case VK_PHYSICAL_DEVICE_TYPE_CPU: return DeviceType::cpu;
		}
	}
	return DeviceType::none;
}

coResult coVulkanLogicalDevice::SupportsGraphics(coBool& _out) const
{
	_out = false;
	coTRY(vulkanPhysicalDevice, nullptr);
	const coUint nbFamilies = vulkanPhysicalDevice->GetNbQueueFamilies();
	for (coUint i = 0; i < nbFamilies; ++i)
	{
		coTRY(vulkanPhysicalDevice->SupportsGraphics(_out, i), nullptr);
		if (_out)
			return true;
	}
	return true;
}

coResult coVulkanLogicalDevice::SupportsSurface(coBool& _out, const coSurface& _surface) const
{
	_out = false;
	coTRY(vulkanPhysicalDevice, nullptr);
	const coUint nbFamilies = vulkanPhysicalDevice->GetNbQueueFamilies();
	for (coUint i = 0; i < nbFamilies; ++i)
	{
		coTRY(vulkanPhysicalDevice->SupportsSurface(_out, i, _surface), nullptr);
		if (_out)
			return true;
	}
	return true;
}