// Copyright(c) 2016 Jounayd Id Salah
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

coResult coEntitySystem::Init(const coEntityHandle& handle)
{
	coEntity* entity = Get(handle);
	coTRY(entity, nullptr);
	coTRY(entity->Init(), nullptr);
	return true;
}

coResult coEntitySystem::Init(const coArray<coEntityHandle>& handles)
{
	for (const coEntityHandle& handle : handles)
	{
		coTRY(Init(handle), nullptr);
	}
	return true;
}

coResult coEntitySystem::Start(const coEntityHandle& handle)
{
	coEntity* entity = Get(handle);
	coTRY(entity, nullptr);
	coTRY(entity->Start(), nullptr);
	return true;
}

coResult coEntitySystem::Start(const coArray<coEntityHandle>& handles)
{
	for (const coEntityHandle& handle : handles)
	{
		coTRY(Start(handle), nullptr);
	}
	return true;
}

coResult coEntitySystem::Save(const coEntityHandle& handle, coBinaryOutputStream& stream)
{
	coEntity* entity = Get(handle);
	if (entity)
		coTRY(entity->Save(stream), nullptr);
	return true;
}

coResult coEntitySystem::Load(const coEntityHandle& handle, coBinaryInputStream& stream)
{
	coEntity* entity = Get(handle);
	if (entity)
		coTRY(entity->Load(stream), nullptr);
	return true;
}

void coEntitySystem::Stop(const coEntityHandle& handle)
{
	coEntity* entity = Get(handle);
	if (entity)
		entity->Stop();
}

void coEntitySystem::Stop(const coArray<coEntityHandle>& handles)
{
	for (const coEntityHandle& handle : handles)
	{
		Stop(handle);
	}
}

void coEntitySystem::Release(const coEntityHandle& handle)
{
	coEntity* entity = Get(handle);
	if (entity)
		entity->Release();
}

void coEntitySystem::Release(const coArray<coEntityHandle>& handles)
{
	for (const coEntityHandle& handle : handles)
	{
		Release(handle);
	}
}