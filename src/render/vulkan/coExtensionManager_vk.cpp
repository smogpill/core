// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coExtensionManager_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult_f.h"

coExtensionManager_vk::~coExtensionManager_vk()
{
	for (auto p : requestedExtensions)
		delete p;
}

coResult coExtensionManager_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(InitSupportedExtensions(), "Failed to init supported layers");
	return true;
}

coResult coExtensionManager_vk::InitSupportedExtensions()
{
	coUint32 nb = 0;
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &nb, nullptr), "Failed to get the supported extension count.");
	coResize(supportedExtensions, nb);
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensions.count, supportedExtensions.data), "Failed to get supported extensions.");
	coTRY(supportedExtensions.count == nb, nullptr);
	return true;
}

coBool coExtensionManager_vk::IsSupported(const coConstString& _extension) const
{
	for (const VkExtensionProperties& supportedExtension : supportedExtensions)
	{
		if (_extension == supportedExtension.extensionName)
			return true;
	}
	return false;
}

coBool coExtensionManager_vk::IsRequested(const coConstString& _extension) const
{
	for (const coDynamicString* s : requestedExtensions)
	{
		coASSERT(s);
		if (*s == _extension)
			return true;
	}
	return false;
}

coResult coExtensionManager_vk::AddRequested(const coConstString& _extension)
{
	coTRY(IsSupported(_extension), "Layer not supported: " << _extension);
	coTRY(!IsRequested(_extension), "Layer already requested: " << _extension);
	coDynamicString* extension = new coDynamicString(_extension);
	coNullTerminate(*extension);
	coPushBack(requestedExtensions, extension);
	return true;
}

coResult coExtensionManager_vk::GetAllRequested(coDynamicArray<const coChar*>& _layers) const
{
	coClear(_layers);
	for (const coDynamicString* s : requestedExtensions)
	{
		coASSERT(coIsNullTerminated(*s));
		coPushBack(_layers, s->data);
	}
	return true;
}
