// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntitySystem.h"
#include "coEntityHandle.h"
#include "coEntity.h"
#include <container/array/coDynamicArray_f.h>

coDEFINE_SINGLETON(coEntitySystem);

coEntitySystem::coEntitySystem()
{
	coEntity* nullEntity = new coEntity();
	Give(*nullEntity);
}

coEntitySystem::~coEntitySystem()
{
	for (coEntity* p : entities)
		delete p;
}

const coEntityHandle& coEntitySystem::Give(coEntity& entity)
{
	coEntityHandle handle;
	handle.val = entities.count;
	entity._OnSetHandle(handle);
	coPushBack(entities, &entity);
	return entity.GetHandle();
}

coEntity* coEntitySystem::Get(const coEntityHandle& handle) const
{
	for (coEntity* entity : entities)
	{
		if (entity->GetHandle() == handle)
			return entity;
	}
	return nullptr;
}

const coEntityHandle& coEntitySystem::Get(const coUuid& uuid) const
{
	for (coEntity* entity : entities)
	{
		if (entity->GetUuid() == uuid)
			return entity->GetHandle();
	}
	return coEntityHandle::empty;
}
