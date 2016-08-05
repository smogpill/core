// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coLayerManager_vk;

coResult coGetNbPhysicalDevices_vk(coUint32& _nb, const VkInstance& _instance_vk);
coResult coGetPhysicalDevices_vk(coDynamicArray<VkPhysicalDevice>& _out, const VkInstance& _instance_vk);

class coDevice_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		VkPhysicalDevice physicalDevice_vk;
		coLayerManager_vk* layerManager_vk;
	};
	coDevice_vk();
	virtual ~coDevice_vk();

	coResult GetScore(coUint32& _out) const;
	const VkQueue& GetQueue() const { return queue_vk; }
	
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coResult InitQueueFamilyIndex();
	coResult InitLogicalDevice();
	coResult InitQueue();

	VkPhysicalDevice physicalDevice_vk;
	VkDevice logicalDevice_vk;
	VkQueue queue_vk;
	coInt32 queueFamilyIndex;
	coLayerManager_vk* layerManager_vk;
};
