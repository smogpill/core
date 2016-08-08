// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderer.h"
#include "container/array/coDynamicArray.h"

class coVulkanMessageHandler;
class coVulkanLayerManager;
class coVulkanExtensionManager;
class coVulkanPhysicalDevice;
class coVulkanLogicalDevice;

class coVulkanRenderer final : public coRenderer
{
	coDECLARE_SUPER(coRenderer);
public:
	coVulkanRenderer();
	virtual ~coVulkanRenderer();

	const VkInstance& GetVkInstance() const { return instance_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitLayers();
	coResult InitExtensions();
	coResult InitInstance();
	coResult InitMessageHandler();
	coResult InitDevices();

	VkInstance instance_vk;
	VkAllocationCallbacks allocator_vk;
	coBool enableDebug;
	coVulkanMessageHandler* messageHandler_vk;
	coVulkanLayerManager* layerManager_vk;
	coVulkanExtensionManager* extensionManager_vk;
	coDynamicArray<coVulkanPhysicalDevice*> physicalDevices;
	coDynamicArray<coVulkanLogicalDevice*> logicalDevices;
};
