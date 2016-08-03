// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

coResult coGetNbAvailableLayers_vk(coUint32& _nbLayers);
coResult coGetAvailableLayers_vk(coDynamicArray<VkLayerProperties>& _validationLayers);
coResult coCheckIfLayersAreAvailable_vk(const coArray<const coChar*>& _requestedLayers);