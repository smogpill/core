// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <pattern/singleton/coSingleton.h>
#include <pattern/uuid/coUuid_f.h>
#include <pattern/lock/coSharedMutex.h>
#include <pattern/lock/coSharedLock.h>
#include <pattern/lock/coScopedLock.h>
#include <container/map/coHashMap.h>
#include <container/map/coHashMap_f.h>
#include "../../entity/coEntityHandle.h"

class coEntityHandle;

class coUuidRegistry
{
	coDECLARE_SINGLETON(coUuidRegistry);
public:
	const coEntityHandle& GetEntity(const coUuid& uuid);
	void SetEntity(const coUuid& uuid, const coEntityHandle& entity);

private:
	coHashMap<coUuid, coEntityHandle, 4096> _uuidToEntity;
	mutable coSharedMutex _mutex;
};

inline const coEntityHandle& coUuidRegistry::GetEntity(const coUuid& uuid)
{
	coSharedLock lock(_mutex);
	return coGet(_uuidToEntity, uuid, coEntityHandle());
}

inline void coUuidRegistry::SetEntity(const coUuid& uuid, const coEntityHandle& entity)
{
	coScopedLock lock(_mutex);
	coSet(_uuidToEntity, uuid, entity);
}
