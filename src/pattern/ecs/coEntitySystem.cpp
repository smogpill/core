// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntitySystem.h"
#include "coEntityHandle.h"
#include "coEntity.h"
#include <container/array/coDynamicArray_f.h>
#include <lang/result/coResult_f.h>

coDEFINE_SINGLETON(coEntitySystem);

coEntitySystem::coEntitySystem()
{
	/*
	coEntity* nullEntity = new coEntity();
	Give(*nullEntity);
	*/
}

coEntitySystem::~coEntitySystem()
{
	for (coEntity* p : entities)
		delete p;
}

void coEntitySystem::Give(coEntity& entity)
{
	coPushBack(entities, &entity);
}

coEntity* coEntitySystem::Get(const coUuid& uuid) const
{
	for (coEntity* entity : entities)
	{
		if (entity->GetUuid() == uuid)
			return entity;
	}
	return nullptr;
}
