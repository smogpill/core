// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

coResult coGetNbAvailableExtensions_vk(coUint32& _nbExtensions);
coResult coGetAvailableExtensions_vk(coDynamicArray<VkExtensionProperties>& _extensions);
coResult coCheckIfExtensionsAreAvailable_vk(const coArray<const coChar*>& _requestedExtensions);
