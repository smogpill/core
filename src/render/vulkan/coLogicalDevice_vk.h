// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coLayerManager_vk;
class coSurface_vk;
class coPhysicalDevice_vk;

class coLogicalDevice_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coPhysicalDevice_vk* physicalDevice_vk;
		coLayerManager_vk* layerManager_vk;
		coSurface_vk* surface_vk;
	};
	coLogicalDevice_vk();
	virtual ~coLogicalDevice_vk();

	coResult AddRequestedExtension(const coConstString& _extension);
	coResult IsExtensionRequested(const coConstString& _extension) const;
	coResult GetScore(coUint32& _out) const;
	const VkQueue& GetGraphicsQueue() const { return graphicsQueue_vk; }
	const VkQueue& GetPresentQueue() const { return presentQueue_vk; }
	const VkDevice& GetVkDevice() const { return logicalDevice_vk; }
	coPhysicalDevice_vk* GetPhysicalDevice() const { return physicalDevice_vk; }
	coInt32 GetGraphicsFamilyIndex() const { return graphicsFamilyIndex; }
	coInt32 GetPresentFamilyIndex() const { return presentFamilyIndex; }
	coResult WaitIdle();
	
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coResult InitQueueFamilyIndices(coSurface_vk* _surface_vk);
	coResult InitLogicalDevice();
	coResult InitQueues();
	coResult GetAllRequestedExtensions(coDynamicArray<const coChar*>& _extensions) const;

	coPhysicalDevice_vk* physicalDevice_vk;
	VkDevice logicalDevice_vk;
	coInt32 graphicsFamilyIndex;
	coInt32 presentFamilyIndex;
	coDynamicArray<coDynamicString*> requestedExtensions;
	coDynamicArray<VkQueue> queues;
	VkQueue graphicsQueue_vk;
	VkQueue presentQueue_vk;
	coLayerManager_vk* layerManager_vk;
};
