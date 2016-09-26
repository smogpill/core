// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDevice.h"

class coVulkanLayerManager;
class coVulkanSurface;
class coVulkanPhysicalDevice;
class coSwapChain;
class coVulkanCommandPool;
class coVulkanDeviceAllocator;

class coVulkanLogicalDevice : public coRenderDevice
{
	coDECLARE_SUPER(coRenderDevice);
public:
	enum QueueType
	{
		graphics,
		compute,
		transfer,
		count
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coVulkanPhysicalDevice* physicalDevice_vk;
		coVulkanLayerManager* layerManager_vk;
	};
	coVulkanLogicalDevice();
	virtual ~coVulkanLogicalDevice();

	coResult AddRequestedExtension(const coConstString& _extension);
	coResult IsExtensionRequested(const coConstString& _extension) const;
	const VkDevice& GetVkDevice() const { return device_vk; }
	coVulkanPhysicalDevice* GetPhysicalDevice() const { return vulkanPhysicalDevice; }
	const VkQueue& GetVkQueue(QueueType _type) const { return queues_vk[_type]; }
	const VkCommandPool& GetVkCommandPool(QueueType _type) const;
	coVulkanCommandPool* GetVulkanCommandPool(QueueType _type) const;
	coResult GetVkQueue(VkQueue& _out, coUint _queueFamilyIndex, coUint _index);
	virtual coResult WaitForIdle() override;
	virtual DeviceType GetDeviceType() const override;
	virtual coResult SupportsGraphics(coBool& _out) const override;
	virtual coResult SupportsSurface(coBool& _out, const coSurface& _surface) const override;
	virtual coResult Submit(const SubmitConfig& _config) override;
	coVulkanDeviceAllocator* GetVulkanDeviceAllocator() const { return vulkanDeviceAllocator; }
	
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coResult InitQueueFamilyIndices();
	coResult InitQueues();
	coResult InitLogicalDevice();
	coResult InitCommandPools();
	coResult InitFences();
	coResult InitAllocator();
	coResult GetAllRequestedExtensions(coDynamicArray<const coChar*>& _extensions) const;
	void Clear();

	coVulkanPhysicalDevice* vulkanPhysicalDevice;
	VkDevice device_vk;
	coInt32 queueFamilyIndices[QueueType::count];
	VkQueue queues_vk[QueueType::count];
	VkFence fences_vk[QueueType::count];
	coDynamicArray<coDynamicString*> requestedExtensions;
	coVulkanLayerManager* vulkanLayerManager;
	coVulkanCommandPool* vulkanCommandPools[QueueType::count];
	coVulkanDeviceAllocator* vulkanDeviceAllocator;
};
