// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coDevice_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coLayerManager_vk.h"
#include "lang/result/coResult_f.h"

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

static coResult coGetNbQueueFamilyProperties(coUint32& _out, const VkPhysicalDevice& _device)
{
	_out = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &_out, nullptr);
	return true;
}

static coResult coGetQueueFamilyProperties(coDynamicArray<VkQueueFamilyProperties>& _out, const VkPhysicalDevice& _device)
{
	coClear(_out);
	coUint32 nb;
	coTRY(coGetNbQueueFamilyProperties(nb, _device), "Failed to get the queue family properties count");

	coDynamicArray<VkQueueFamilyProperties> a;
	coResize(a, nb);
	vkGetPhysicalDeviceQueueFamilyProperties(_device, &a.count, a.data);
	coTRY(a.count == nb, nullptr);
	coSwap(a, _out);
	return true;
}

coDevice_vk::coDevice_vk()
	: physicalDevice_vk(VK_NULL_HANDLE)
	, logicalDevice_vk(VK_NULL_HANDLE)
	, queue_vk(VK_NULL_HANDLE)
	, queueFamilyIndex(-1)
	, layerManager_vk(nullptr)
{

}

coDevice_vk::~coDevice_vk()
{
	vkDestroyDevice(logicalDevice_vk, nullptr);
}

coDevice_vk::InitConfig::InitConfig()
	: physicalDevice_vk(VK_NULL_HANDLE)
{

}

coResult coDevice_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coASSERT(physicalDevice_vk == VK_NULL_HANDLE);
	coTRY(config.physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	physicalDevice_vk = config.physicalDevice_vk;
	layerManager_vk = config.layerManager_vk;
	coTRY(InitQueueFamilyIndex(), "Failed to init the queue family index");
	return true;
}

coResult coDevice_vk::OnStart()
{
	coTRY(Super::OnStart(), nullptr);

	coTRY(InitLogicalDevice(), "Failed to init the logical device.");
	coTRY(InitQueue(), "Failed to init the queue.");
	return true;
}

void coDevice_vk::OnStop()
{
	queue_vk = VK_NULL_HANDLE;
	vkDestroyDevice(logicalDevice_vk, nullptr);
	logicalDevice_vk = VK_NULL_HANDLE;
	Super::OnStop();
}

coResult coDevice_vk::InitQueueFamilyIndex()
{
	queueFamilyIndex = -1;

	coDynamicArray<VkQueueFamilyProperties> queueFamilyProps;
	coTRY(coGetQueueFamilyProperties(queueFamilyProps, physicalDevice_vk), "Failed to get the queue family properties");
	for (coUint i = 0; i < queueFamilyProps.count; ++i)
	{
		const VkQueueFamilyProperties& prop = queueFamilyProps[i];
		if (prop.queueCount > 0 && prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueFamilyIndex = i;
			break;
		}
	}
	return true;
}

coResult coDevice_vk::InitLogicalDevice()
{
	coTRY(queueFamilyIndex >= 0, nullptr);
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	coFloat priority = 1.0f;
	queueCreateInfo.pQueuePriorities = &priority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	coDynamicArray<const coChar*> layers;
	if (layerManager_vk)
	{
		layerManager_vk->GetAllRequested(layers);
	}

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = layers.count;
	createInfo.ppEnabledLayerNames = layers.data;

	coTRY(logicalDevice_vk == VK_NULL_HANDLE, nullptr);
	coTRY(vkCreateDevice(physicalDevice_vk, &createInfo, nullptr, &logicalDevice_vk), "Failed to create device");

	return true;
}

coResult coDevice_vk::InitQueue()
{
	coTRY(queue_vk == VK_NULL_HANDLE, nullptr);
	coTRY(queueFamilyIndex >= 0, nullptr);
	vkGetDeviceQueue(logicalDevice_vk, queueFamilyIndex, 0, &queue_vk);
	return true;
}

coResult coDevice_vk::GetScore(coUint32& _out) const
{
	_out = 0;

	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice_vk, &deviceProperties);
	if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		return true;
	}

	if (queueFamilyIndex < 0)
		return true;

	_out = 1;

	return true;
}
