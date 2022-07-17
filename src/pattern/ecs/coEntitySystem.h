// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityHandle.h"
#include "../singleton/coSingleton.h"
#include <container/array/coDynamicArray.h>
#include <container/map/coHashMap.h>
#include <lang/result/coResult.h>
#include <pattern/uuid/coUuid.h>
class coEntity;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntitySystem
{
	coDECLARE_SINGLETON(coEntitySystem);
public:
	void SetUuid(const coUuid& uuid, coEntity* entity);
	coEntity* GetFromUuid(const coUuid& uuid) const;

private:
	coHashMap<coUuid, coEntity*, 4096> uuidToEntity;
};
