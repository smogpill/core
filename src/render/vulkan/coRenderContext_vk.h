// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coMessageHandler_vk;
class coLayerManager_vk;
class coDevice_vk;
class coExtensionManager_vk;

class coRenderContext_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderContext_vk();
	virtual ~coRenderContext_vk();

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
	coMessageHandler_vk* messageHandler_vk;
	coLayerManager_vk* layerManager_vk;
	coExtensionManager_vk* extensionManager_vk;
	coDynamicArray<coDevice_vk*> devices;
};
