// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coSurface_vk;

class coPhysicalDevice_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		VkPhysicalDevice physicalDevice_vk;
	};
	coPhysicalDevice_vk();

	const coArray<VkQueueFamilyProperties>& GetQueueFamilyProperties() const { return queueFamilyProperties; }
	const coArray<VkExtensionProperties>&  GetSupportedExtensions() const { return supportedExtensions; }
	const VkPhysicalDevice& GetVkPhysicalDevice() const { return physicalDevice_vk; }
	const VkPhysicalDeviceProperties& GetProperties() const { return props_vk; }
	coBool IsExtensionSupported(const coConstString& _extension) const;
	coBool SupportsSurface(coUint queueFamilyIndex, const coSurface_vk& _surface_vk) const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitQueueFamilyProperties();
	coResult InitSupportedExtensions();
	coResult InitProps();

	VkPhysicalDevice physicalDevice_vk;
	coDynamicArray<VkQueueFamilyProperties> queueFamilyProperties;
	coDynamicArray<VkExtensionProperties> supportedExtensions;
	VkPhysicalDeviceProperties props_vk;
};

coResult coGetPhysicalDevices(coDynamicArray<coPhysicalDevice_vk*>& _out, const VkInstance& _instance_vk);
