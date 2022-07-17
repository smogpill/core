// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntitySystem.h"
#include "coEntityHandle.h"
#include "coEntity.h"
#include <container/array/coDynamicArray_f.h>
#include <container/map/coHashMap_f.h>
#include <lang/result/coResult_f.h>

coDEFINE_SINGLETON(coEntitySystem);

void coEntitySystem::SetUuid(const coUuid& uuid, coEntity* entity)
{
	coASSERT(uuid.IsValid());
	if (uuid.IsValid()) // security
	{
		coSet(uuidToEntity, uuid, entity);
	}
}

coEntity* coEntitySystem::GetFromUuid(const coUuid& uuid) const
{
	return coGet(uuidToEntity, uuid, static_cast<coEntity*>(nullptr));
}
