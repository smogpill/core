// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPhysicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render_vulkan/coVulkanLayerManager.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanSemaphore.h"
#include "render_vulkan/coVulkanCommandPool.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanDeviceAllocator.h"
#include "render_vulkan/coVulkanDescriptorPool.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "pattern/scope/coDefer.h"

coVulkanLogicalDevice::coVulkanLogicalDevice()
	: vulkanPhysicalDevice(nullptr)
	, device_vk(VK_NULL_HANDLE)
	//, queues_vk{ VK_NULL_HANDLE }
	, vulkanLayerManager(nullptr)
	, vulkanDeviceAllocator(nullptr)
	, vulkanDescriptorPool(nullptr)
{
	for (auto& x : queueFamilyIndices)
		x = -1;
	for (auto& x : queues_vk)
		x = VK_NULL_HANDLE;
	for (auto& x : fences_vk)
		x = VK_NULL_HANDLE;
	for (auto& x : vulkanCommandPools)
		x = nullptr;
}

coVulkanLogicalDevice::~coVulkanLogicalDevice()
{
	Stop();
	Clear();
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
	coTRY(InitAllocator(), "Failed to init the allocator.");
	coTRY(InitQueues(), "Failed to init queues.");
	coTRY(InitCommandPools(), "Failed to init command pools.");
	coTRY(InitDescriptorPools(), "Failed to init descriptor pools.");
	coTRY(InitFences(), "Failed to init fences.");
	return true;
}

void coVulkanLogicalDevice::OnStop()
{
	Clear();
	Super::OnStop();
}

void coVulkanLogicalDevice::Clear()
{
	coCHECK(WaitForIdle(), nullptr);
	for (auto& p : vulkanCommandPools)
	{
		delete p;
		p = nullptr;
	}

	for (VkQueue& queue_vk : queues_vk)
		queue_vk = VK_NULL_HANDLE;
	for (VkFence& fence_vk : fences_vk)
	{
		if (fence_vk != VK_NULL_HANDLE)
		{
			vkDestroyFence(device_vk, fence_vk, nullptr);
			fence_vk = VK_NULL_HANDLE;
		}
	}

	delete vulkanDeviceAllocator;
	vulkanDeviceAllocator = nullptr;
	
	if (device_vk != VK_NULL_HANDLE)
	{
		vkDestroyDevice(device_vk, nullptr);
		device_vk = VK_NULL_HANDLE;
	}
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
			uint32_t queueFamilyIndex_vk;
			coNumericConvert(queueFamilyIndex_vk, queueFamilyIndex);
			coTRY(GetVkQueue(queues_vk[i], queueFamilyIndex_vk, 0), nullptr);
		}
		else
		{
			queues_vk[i] = VK_NULL_HANDLE;
		}
	}
	return true;
}

coResult coVulkanLogicalDevice::InitCommandPools()
{
	coVulkanCommandPool* cp = new coVulkanCommandPool();
	coDEFER() { delete cp; };
	coTRY(queueFamilyIndices[QueueType::graphics] >= 0, nullptr);
	coVulkanCommandPool::InitConfig c;
	c.device = this;
	c.queueFamilyIndex = queueFamilyIndices[QueueType::graphics];
	coTRY(cp->Init(c), "Failed to init command pool.");
	coASSERT(!vulkanCommandPools[QueueType::graphics]);
	coSwap(vulkanCommandPools[QueueType::graphics], cp);
	return true;
}

coResult coVulkanLogicalDevice::InitDescriptorPools()
{
	coVulkanDescriptorPool* dp = new coVulkanDescriptorPool();
	coDEFER() { delete dp; };
	coVulkanDescriptorPool::InitConfig c;
	c.device = this;
	coTRY(dp->Init(c), "Failed to init descriptor pool.");
	coASSERT(!vulkanDescriptorPool);
	coSwap(vulkanDescriptorPool, dp);
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

	VkPhysicalDeviceFeatures deviceFeatures{};

	coDynamicArray<const coChar*> layers;
	if (vulkanLayerManager)
		coTRY(vulkanLayerManager->GetAllRequested(layers), nullptr);

	coDynamicArray<const coChar*> extensions;
	coTRY(GetAllRequestedExtensions(extensions), nullptr);

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = extensions.count;
	createInfo.ppEnabledExtensionNames = extensions.data;
	createInfo.enabledLayerCount = layers.count;
	createInfo.ppEnabledLayerNames = layers.data;

	coTRY(device_vk == VK_NULL_HANDLE, nullptr);
	const VkPhysicalDevice& physicalDevice_vk = vulkanPhysicalDevice->GetVkPhysicalDevice();
	coTRY(physicalDevice_vk != VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkCreateDevice(physicalDevice_vk, &createInfo, nullptr, &device_vk), "Failed to create the logical device.");

	return true;
}

coResult coVulkanLogicalDevice::GetVkQueue(VkQueue& _out, coUint _queueFamilyIndex, coUint _index)
{
	_out = VK_NULL_HANDLE;
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
	coTRY(Super::WaitForIdle(), nullptr);
	if (device_vk != VK_NULL_HANDLE)
	{
		coVULKAN_TRY(vkDeviceWaitIdle(device_vk), "Wait device for idle failed.");
	}
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

coResult coVulkanLogicalDevice::Submit(const SubmitConfig& _config)
{
	coTRY(Super::Submit(_config), nullptr);

	// Wait semaphores
	coDynamicArray<VkSemaphore> waitSemaphores_vk;
	coDynamicArray<VkPipelineStageFlags> waitFlags_vk;
	coReserve(waitSemaphores_vk, _config.waitSemaphores.count);
	coReserve(waitFlags_vk, _config.waitSemaphores.count);
	for (const coRenderSemaphore* semaphore : _config.waitSemaphores)
	{
		const coVulkanSemaphore* vulkanSemaphore = static_cast<const coVulkanSemaphore*>(semaphore);
		coTRY(vulkanSemaphore, nullptr);
		const VkSemaphore& semaphore_vk = vulkanSemaphore->GetVkSemaphore();
		coTRY(semaphore_vk != VK_NULL_HANDLE, nullptr);
		coPushBack(waitSemaphores_vk, semaphore_vk);
		coPushBack(waitFlags_vk, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	}

	// Finish semaphores
	coDynamicArray<VkSemaphore> finishSemaphores_vk;
	coReserve(finishSemaphores_vk, _config.finishSemaphores.count);
	for (const coRenderSemaphore* semaphore : _config.finishSemaphores)
	{
		const coVulkanSemaphore* vulkanSemaphore = static_cast<const coVulkanSemaphore*>(semaphore);
		coTRY(vulkanSemaphore, nullptr);
		const VkSemaphore& semaphore_vk = vulkanSemaphore->GetVkSemaphore();
		coTRY(semaphore_vk != VK_NULL_HANDLE, nullptr);
		coPushBack(finishSemaphores_vk, semaphore_vk);
	}

	// Command buffers
	coDynamicArray<VkCommandBuffer> commandBuffers_vk;
	coReserve(commandBuffers_vk, _config.commandBuffers.count);
	for (const coRenderCommandBuffer* commandBuffer : _config.commandBuffers)
	{
		const coVulkanCommandBuffer* vulkanCommandBuffer = static_cast<const coVulkanCommandBuffer*>(commandBuffer);
		coTRY(vulkanCommandBuffer, nullptr);
		const VkCommandBuffer& commandBuffer_vk = vulkanCommandBuffer->GetVkCommandBuffer();
		coTRY(commandBuffer_vk != VK_NULL_HANDLE, nullptr);
		coPushBack(commandBuffers_vk, commandBuffer_vk);
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = waitSemaphores_vk.count;
	submitInfo.pWaitSemaphores = waitSemaphores_vk.data;
	submitInfo.pWaitDstStageMask = waitFlags_vk.data;
	submitInfo.commandBufferCount = commandBuffers_vk.count;
	submitInfo.pCommandBuffers = commandBuffers_vk.data;
	submitInfo.signalSemaphoreCount = finishSemaphores_vk.count;
	submitInfo.pSignalSemaphores = finishSemaphores_vk.data;

	const VkQueue graphicsQueue = queues_vk[QueueType::graphics];
	coTRY(graphicsQueue != VK_NULL_HANDLE, nullptr);

	const VkFence& fence_vk = fences_vk[QueueType::graphics];
	VkResult res;
	do
	{
		res = vkWaitForFences(device_vk, 1, &fence_vk, VK_TRUE, 100000000);
	} while (res == VK_TIMEOUT);
	coVULKAN_TRY(res, "Failed to wait for Vulkan fence.");

	coVULKAN_TRY(vkResetFences(device_vk, 1, &fence_vk), "Failed to reset a Vulkan fence.");
	coVULKAN_TRY(vkQueueSubmit(graphicsQueue, 1, &submitInfo, fence_vk), "Failed to submit command buffers.");

	return true;
}

const VkCommandPool& coVulkanLogicalDevice::GetVkCommandPool(QueueType _type) const
{
	static VkCommandPool nullCommandPool_vk = VK_NULL_HANDLE;
	const coVulkanCommandPool* vulkanCommandPool = vulkanCommandPools[_type];
	return vulkanCommandPool ? vulkanCommandPool->GetVkCommandPool() : nullCommandPool_vk;
}

coVulkanCommandPool* coVulkanLogicalDevice::GetVulkanCommandPool(QueueType _type) const
{
	return vulkanCommandPools[_type];
}

coResult coVulkanLogicalDevice::InitFences()
{
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);

	VkFenceCreateInfo createInfo_vk{};
	createInfo_vk.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo_vk.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (auto& f : fences_vk)
	{
		coVULKAN_TRY(vkCreateFence(device_vk, &createInfo_vk, nullptr, &f), "Failed to create Vulkan fence.");
	}
	
	return true;
}

coResult coVulkanLogicalDevice::InitAllocator()
{
	coASSERT(device_vk);

	coVulkanDeviceAllocator* a = new coVulkanDeviceAllocator();
	coDEFER() { delete a; };
	coVulkanDeviceAllocator::InitConfig c;
	c.device = this;
	coTRY(a->Init(c), "Failed to init the device allocator.");
	coSwap(vulkanDeviceAllocator, a);
	return true;
}