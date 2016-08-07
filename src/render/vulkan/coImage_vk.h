// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coInt32x3.h"

class coLogicalDevice_vk;

class coImage_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum Type
	{
		default,
		staging,
		dynamic
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coLogicalDevice_vk* device_vk;
		coInt32x3 size;
		coUint32 arraySize;
		VkDeviceMemory deviceMemory_vk;
		VkDeviceSize deviceMemoryOffset_vk;
		//coBool cpuWriteAccess;
		//coBool cpuReadAccess;
		Type type;
	};
	coImage_vk();
	virtual ~coImage_vk();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	static coResult ComputeImageType(VkImageType& _out, const InitConfig& _config);

	coLogicalDevice_vk* device_vk;
	VkDeviceMemory deviceMemory_vk;
	VkImage image_vk;
};
