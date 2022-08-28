// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coECSConfig.h"
#include "../singleton/coSingleton.h"
#include <container/array/coDynamicArray.h>
#include <container/map/coHashMap.h>
#include <lang/result/coResult.h>
#include <pattern/uuid/coUuid.h>

class coComponentMask;

class coECS
{
	coDECLARE_SINGLETON(coECS);
public:
	coComponentMaskHandle GetOrCreateComponentMask(const coComponentMask&);
	const coComponentMask& GetComponentMask(const coComponentMaskHandle&) const;
	coUint GetNbComponentMasks() const;
private:
};
