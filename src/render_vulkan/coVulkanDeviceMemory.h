// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coVulkanLogicalDevice;

class coVulkanDeviceMemory : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum Type
	{
		default, // GPU: Read/Write, Stage: Read/Write
		immutable, // GPU: Read, Stage: Read
		dynamic, // CPU: Write, GPU: Read, Stage: Read
		staging // CPU: Read/Write, GPU: Read/Write
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coUint64 size;
		Type type;
		coVulkanLogicalDevice* device_vk;
	};

	coVulkanDeviceMemory();
	virtual ~coVulkanDeviceMemory();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coVulkanLogicalDevice* logicalDevice_vk;
	VkDeviceMemory deviceMemory_vk;
};
