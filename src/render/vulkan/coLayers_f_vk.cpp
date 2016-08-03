// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coLayers_f_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"

coResult coGetNbAvailableLayers_vk(coUint32& _nbLayers)
{
	_nbLayers = 0;
	coTRY_vk(vkEnumerateInstanceLayerProperties(&_nbLayers, nullptr), "Failed to get the layer count.");
	return true;
}

coResult coGetAvailableLayers_vk(coDynamicArray<VkLayerProperties>& _validationLayers)
{
	coUint32 nbLayers;
	coTRY(coGetNbAvailableLayers_vk(nbLayers), nullptr);
	coResize(_validationLayers, nbLayers);
	coTRY_vk(vkEnumerateInstanceLayerProperties(&nbLayers, _validationLayers.data), "Failed to get available layers.");
	return true;
}

coResult coCheckIfLayersAreAvailable_vk(const coArray<const coChar*>& _requestedLayers)
{
	coDynamicArray<VkLayerProperties> availableLayers;
	coTRY(coGetAvailableLayers_vk(availableLayers), "Failed to get available layers");

	coBool missingLayers = false;
	for (const coConstString& requested : _requestedLayers)
	{
		coBool found = false;
		for (const VkLayerProperties& available : availableLayers)
		{
			if (requested == available.layerName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			coWARN("Vulkan layer not available: " << requested);
			missingLayers = true;
		}
	}

	return !missingLayers;
}
