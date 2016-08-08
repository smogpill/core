// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanLayerManager.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"
#include "container/array/coDynamicArray_f.h"

coVulkanLayerManager::~coVulkanLayerManager()
{
	for (auto p : requestedLayers)
		delete p;
}

coResult coVulkanLayerManager::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(InitSupportedLayers(), "Failed to init supported layers");
	return true;
}

coResult coVulkanLayerManager::InitSupportedLayers()
{
	coUint32 nb = 0;
	coVULKAN_TRY(vkEnumerateInstanceLayerProperties(&nb, nullptr), "Failed to get the supported layer count.");
	coResize(supportedLayers, nb);
	coVULKAN_TRY(vkEnumerateInstanceLayerProperties(&supportedLayers.count, supportedLayers.data), "Failed to get supported layers.");
	coTRY(supportedLayers.count == nb, nullptr);
	return true;
}

coBool coVulkanLayerManager::IsSupported(const coConstString& _layer) const
{
	for (const VkLayerProperties& supportedLayer : supportedLayers)
	{
		if (_layer == supportedLayer.layerName)
			return true;
	}
	return false;
}

coBool coVulkanLayerManager::IsRequested(const coConstString& _layer) const
{
	for (const coDynamicString* s : requestedLayers)
	{
		coASSERT(s);
		if (*s == _layer)
			return true;
	}
	return false;
}

coResult coVulkanLayerManager::AddRequested(const coConstString& _layerName)
{
	coTRY(IsSupported(_layerName), "Layer not supported: " << _layerName);
	coTRY(!IsRequested(_layerName), "Layer already requested: " << _layerName);
	coDynamicString* layer = new coDynamicString(_layerName);
	coNullTerminate(*layer);
	coPushBack(requestedLayers, layer);
	return true;
}

coResult coVulkanLayerManager::GetAllRequested(coDynamicArray<const coChar*>& _layers) const
{
	coClear(_layers);
	for (const coDynamicString* s : requestedLayers)
	{
		coASSERT(coIsNullTerminated(*s));
		coPushBack(_layers, s->data);
	}
	return true;
}
