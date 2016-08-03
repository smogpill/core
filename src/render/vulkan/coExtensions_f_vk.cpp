// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coExtensions_f_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "lang/result/coResult_f.h"

coResult coGetNbAvailableExtensions_vk(coUint32& _nbExtensions)
{
	_nbExtensions = 0;
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &_nbExtensions, nullptr), "Failed to get the extension count.");
	return true;
}

coResult coGetAvailableExtensions_vk(coDynamicArray<VkExtensionProperties>& _extensions)
{
	coUint32 nbExtensions = 0;
	coTRY(coGetNbAvailableExtensions_vk(nbExtensions), nullptr);
	coResize(_extensions, nbExtensions);
	coTRY_vk(vkEnumerateInstanceExtensionProperties(nullptr, &nbExtensions, _extensions.data), "Failed to get the extension properties");
	return true;
}

coResult coCheckIfExtensionsAreAvailable_vk(const coArray<const coChar*>& _requestedExtensions)
{
	coDynamicArray<VkExtensionProperties> availableExtensions;
	coTRY(coGetAvailableExtensions_vk(availableExtensions), "Failed to get available extensions");

	coBool missingExtensions = false;
	for (const coConstString& requested : _requestedExtensions)
	{
		coBool found = false;
		for (const VkExtensionProperties& available : availableExtensions)
		{
			if (requested == available.extensionName)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			coWARN("Vulkan extension not available: " << requested);
			missingExtensions = true;
		}
	}

	return !missingExtensions;
}