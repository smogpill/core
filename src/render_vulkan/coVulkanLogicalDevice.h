// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDevice.h"

class coVulkanLayerManager;
class coVulkanSurface;
class coVulkanPhysicalDevice;
class coSwapChain;

class coVulkanLogicalDevice : public coRenderDevice
{
	coDECLARE_SUPER(coRenderDevice);
public:
	enum QueueType
	{
		graphics,
		present,
		count
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coVulkanPhysicalDevice* physicalDevice_vk;
		coVulkanLayerManager* layerManager_vk;
		coVulkanSurface* surface_vk;
	};
	coVulkanLogicalDevice();
	virtual ~coVulkanLogicalDevice();

	coResult AddRequestedExtension(const coConstString& _extension);
	coResult IsExtensionRequested(const coConstString& _extension) const;
	coResult GetScore(coUint32& _out) const;
	//const VkQueue& GetGraphicsQueue() const { return graphicsQueue_vk; }
	//const VkQueue& GetPresentQueue() const { return presentQueue_vk; }
	const VkDevice& GetVkDevice() const { return logicalDevice_vk; }
	coVulkanPhysicalDevice* GetPhysicalDevice() const { return physicalDevice_vk; }
	coInt64 GetQueueId(QueueType _type) const { return queueIds[_type]; }
	static coInt32 GetQueueFamilyIndex(coInt64 _queueId);
	coResult WaitForIdle();
	coResult Present(const coArray<coSwapChain*> _swapChains);
	
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coResult InitQueueFamilyIndices(coVulkanSurface* _surface_vk);
	coResult InitLogicalDevice();
	coResult InitQueues();
	coResult GetAllRequestedExtensions(coDynamicArray<const coChar*>& _extensions) const;

	coVulkanPhysicalDevice* physicalDevice_vk;
	VkDevice logicalDevice_vk;
	coInt64 queueIds[QueueType::count];
	//VkQueue queues_vk[QueueType::count];
	coDynamicArray<coDynamicString*> requestedExtensions;
	coVulkanLayerManager* layerManager_vk;
};
