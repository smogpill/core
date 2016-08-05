// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coMessageHandler_vk;
class coLayerManager_vk;
class coDevice_vk;
class coExtensionManager_vk;

class coRendererInfo_vk
{
public:
	coRendererInfo_vk();
	VkInstance instance_vk;
	VkAllocationCallbacks allocator_vk;
	coBool enableDebug;
	coMessageHandler_vk* messageHandler_vk;
	coLayerManager_vk* layerManager_vk;
	coExtensionManager_vk* extensionManager_vk;
	coDynamicArray<coDevice_vk*> devices;
};
