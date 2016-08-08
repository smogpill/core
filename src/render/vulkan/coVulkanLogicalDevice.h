// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coVulkanLayerManager;
class coVulkanSurface;
class coVulkanPhysicalDevice;

class coVulkanLogicalDevice : public coObject
{
	coDECLARE_SUPER(coObject);
public:
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
	const VkQueue& GetGraphicsQueue() const { return graphicsQueue_vk; }
	const VkQueue& GetPresentQueue() const { return presentQueue_vk; }
	const VkDevice& GetVkDevice() const { return logicalDevice_vk; }
	coVulkanPhysicalDevice* GetPhysicalDevice() const { return physicalDevice_vk; }
	coInt32 GetGraphicsFamilyIndex() const { return graphicsFamilyIndex; }
	coInt32 GetPresentFamilyIndex() const { return presentFamilyIndex; }
	coResult WaitIdle();
	
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
	coInt32 graphicsFamilyIndex;
	coInt32 presentFamilyIndex;
	coDynamicArray<coDynamicString*> requestedExtensions;
	coDynamicArray<VkQueue> queues;
	VkQueue graphicsQueue_vk;
	VkQueue presentQueue_vk;
	coVulkanLayerManager* layerManager_vk;
};
