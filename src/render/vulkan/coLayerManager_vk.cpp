// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coLayerManager_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "container/array/coDynamicArray_f.h"

coLayerManager_vk::~coLayerManager_vk()
{
	for (auto p : requestedLayers)
		delete p;
}

coResult coLayerManager_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(InitSupportedLayers(), "Failed to init supported layers");
	return true;
}

coResult coLayerManager_vk::InitSupportedLayers()
{
	coUint32 nb = 0;
	coTRY_vk(vkEnumerateInstanceLayerProperties(&nb, nullptr), "Failed to get the supported layer count.");
	coResize(supportedLayers, nb);
	coTRY_vk(vkEnumerateInstanceLayerProperties(&supportedLayers.count, supportedLayers.data), "Failed to get supported layers.");
	coTRY(supportedLayers.count == nb, nullptr);
	return true;
}

coBool coLayerManager_vk::IsSupported(const coConstString& _layer) const
{
	for (const VkLayerProperties& supportedLayer : supportedLayers)
	{
		if (_layer == supportedLayer.layerName)
			return true;
	}
	return false;
}

coBool coLayerManager_vk::IsRequested(const coConstString& _layer) const
{
	for (const coDynamicString* s : requestedLayers)
	{
		coASSERT(s);
		if (*s == _layer)
			return true;
	}
	return false;
}

coResult coLayerManager_vk::AddRequested(const coConstString& _layerName)
{
	coTRY(IsSupported(_layerName), "Layer not supported: " << _layerName);
	coTRY(!IsRequested(_layerName), "Layer already requested: " << _layerName);
	coDynamicString* layer = new coDynamicString(_layerName);
	coNullTerminate(*layer);
	coPushBack(requestedLayers, layer);
	return true;
}

coResult coLayerManager_vk::GetAllRequested(coDynamicArray<const coChar*>& _layers) const
{
	coClear(_layers);
	for (const coDynamicString* s : requestedLayers)
	{
		coASSERT(coIsNullTerminated(*s));
		coPushBack(_layers, s->data);
	}
	return true;
}
