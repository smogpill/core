// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coECS.h"
#include <debug/log/coAssert.h>
#include <container/array/coDynamicArray_f.h>
#include <io/archive/coArchive.h>
#include "entity/coEntityType.h"
#include "storage/coEntityPackStorage.h"
#include "component/coComponentIterator.h"
#include "component/coComponentType.h"
#include "lib/uuid/coUuidRegistry.h"

coDEFINE_SINGLETON(coECS);

coECS::coECS()
{
	coUuidRegistry::CreateInstanceIfMissing();
}

coECS::~coECS()
{
	// TODO destroy all entities
}

coEntityHandle coECS::CreateEntity(const coType& type)
{
	coLockScope l(lock);
	const coEntityHandle handle = CreateEmptyEntity();
	coEntity& entity = _GetEntity(handle.index);
	entity.entityType = static_cast<const coEntityType*>(type.customTypeData);
	entity.CreateComponents();
	return handle;
}

coEntityHandle coECS::CreateEmptyEntity()
{
	coLockScope l(lock);
	if (freeEntities.count == 0)
	{
		const coUint32 blockIndex = entityBlocks.count;
		EntityBlock* block = new EntityBlock();
		coPushBack(entityBlocks, block);
		const coUint32 nbAdded = 256;
		coResize(freeEntities, nbEntitiesPerBlock);
		const coUint32 startIndex = blockIndex << blockIndexShift;
		for (coUint32 index = 0; index < nbEntitiesPerBlock; ++index)
			freeEntities[index] = startIndex + index;
	}
	const coUint32 index = coPopBack(freeEntities);
	coEntity& entity = _GetEntity(index);
	const coUint32 generation = entity.generation;
	new (&entity) coEntity();
	entity.index = index;
	entity.generation = generation;
	return coEntityHandle(index, entity.generation);
}

void* coECS::GetComponent(const coEntityHandle& entityHandle, const coType& type) const
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(entityHandle);
	if (entity)
	{
		return entity->GetComponent(type);
	}
	else
	{
		return nullptr;
	}
}

void coECS::DestroyEntity(const coEntityHandle& entityHandle)
{
	if (!entityHandle.IsValid())
		return;
	coASSERT(IsAlive(entityHandle));
	DestroyEntity(entityHandle.index);
}

void coECS::DestroyEntity(coUint32 index)
{
	coLockScope l(lock);
	coEntity& entity = _GetEntity(index);

	// Destroy children
	auto destroyChild = [&](const coEntity& child)
	{
		DestroyEntity(child.index);
		return true;
	};
	_ReverseVisitChildren(entity, destroyChild);

	entity.SetState(coEntity::State::NONE);
	entity.~coEntity();
	++entity.generation;
	coPushBack(freeEntities, index);
}

coEntityHandle coECS::Clone(const coEntityHandle& entityHandle)
{
	coLockScope l(lock);
	const coEntity* source = GetEntity(entityHandle);
	if (source == nullptr)
		return coEntityHandle();

	coASSERT(source->state == coEntity::State::NONE); // Other states not supported yet

	const coEntityHandle targetHandle = CreateEmptyEntity();
	coEntity& target = _GetEntity(targetHandle.index);
	target.entityType = source->entityType;
	target.CreateComponents(*source);

	auto func = [&](const coEntity& sourceChild)
	{
		const coEntityHandle targetChild = Clone(sourceChild.GetHandle());
		SetParent(targetChild, targetHandle);
		return true;
	};
	_VisitChildren(*source, func);
	
	return targetHandle;
}

void coECS::SaveEntity(coArchive& archive, const coEntityHandle& handle) const
{
	coLockScope l(lock);
	if (!handle.IsValid())
		return;
	coASSERT(IsAlive(handle));
	coEntityPackStorage storage;
	const coUint nbEntities = GetNbEntities(handle);
	coReserve(storage.entities, nbEntities);
	SaveEntity(storage, handle.index, coUint32(-1));
	archive.WriteRoot(storage);
}

void coECS::SaveEntity(coEntityPackStorage& packStorage, coUint32 entityIndex, coUint32 parentEntityStorageIndex) const
{
	coLockScope l(lock);
	const coEntity& entity = _GetEntity(entityIndex);
	const coUint32 storageIndex = packStorage.entities.count;
	coEntityStorage& entityStorage = coPushBack(packStorage.entities);
	entityStorage.index = entityIndex;
	entityStorage.parent = parentEntityStorageIndex;
	entityStorage.typeUID = entity.entityType->type->uid;

	auto func = [&](const coEntity& entity)
	{
		SaveEntity(packStorage, entity.index, storageIndex);
		return true;
	};
	_VisitChildren(entity, func);
}

coEntityHandle coECS::LoadEntity(const coArchive& archive)
{
	coLockScope l(lock);
	const coEntityPackStorage* packStorage = archive.CreateObjects<coEntityPackStorage>();
	if (packStorage == nullptr)
		return coEntityHandle();

	const auto& entityStorages = packStorage->entities;

	// Attach to parents
	for (coUint32 entityIdx = 1; entityIdx < entityStorages.count; ++entityIdx)
	{
		const coEntityStorage& entityStorage = entityStorages[entityIdx];
		const coEntityStorage& parentStorage = entityStorages[entityStorage.parent];
		const coEntity& entity = _GetEntity(entityStorage.index);
		const coEntity& parent = _GetEntity(parentStorage.index);
		SetParent(entity.GetHandle(), parent.GetHandle());
	}

	// Get root handle
	coEntityHandle rootHandle;
	if (packStorage->entities.count)
	{
		const coEntity& rootEntity = _GetEntity(packStorage->entities[0].index);
		rootHandle = rootEntity.GetHandle();
	}

	delete packStorage;
	return rootHandle;
}

void coECS::SetParent(const coEntityHandle& childHandle, const coEntityHandle& parentHandle)
{
	coLockScope l(lock);
	coEntity* child = GetEntity(childHandle);
	coASSERT(child);
	if (child->parent != coUint32(-1))
	{
		coEntity& previousSibling = _GetEntity(child->previousSibling);
		coASSERT(previousSibling.nextSibling == childHandle.index);
		previousSibling.nextSibling = child->nextSibling;
		coEntity& nextSibling = _GetEntity(child->nextSibling);
		coASSERT(nextSibling.previousSibling == childHandle.index);
		nextSibling.previousSibling = child->previousSibling;
		child->previousSibling = coUint32(-1);
		child->nextSibling = coUint32(-1);
	}
	coEntity* parent = GetEntity(parentHandle);
	child->parent = parent ? parentHandle.index : coUint32(-1);
	if (parentHandle.IsValid())
	{
		child->parent = parentHandle.index;
		coASSERT(parent);
		if (parent->firstChild == coUint32(-1))
		{
			parent->firstChild = childHandle.index;
			child->nextSibling = childHandle.index;
			child->previousSibling = childHandle.index;
		}
		else
		{
			coEntity& firstChild = _GetEntity(parent->firstChild);
			const coUint32 lastChildIdx = firstChild.previousSibling;
			coEntity& lastChild = _GetEntity(lastChildIdx);
			firstChild.previousSibling = childHandle.index;
			lastChild.nextSibling = childHandle.index;
			child->previousSibling = lastChildIdx;
			child->nextSibling = parent->firstChild;
		}
	}
}

void coECS::Init(const coEntityHandle& handle)
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity && entity->GetState() == coEntity::State::NONE)
	{
		entity->SetState(coEntity::State::INITIALIZED);
	}
}

void coECS::Shutdown(const coEntityHandle& handle)
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity)
	{
		entity->SetState(coEntity::State::NONE);
	}
}

void coECS::SetStarted(const coEntityHandle& handle, coBool b)
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity)
	{
		entity->SetState(b ? coEntity::State::STARTED : coEntity::State::INITIALIZED);
	}
}

coBool coECS::IsAlive(const coEntityHandle& handle) const
{
	coLockScope l(lock);
	return GetEntity(handle) != nullptr;
}

coEntity* coECS::GetEntity(const coEntityHandle& handle) const
{
	if (handle.IsValid())
	{
		const coEntity& entity = _GetEntity(handle.index);
		return entity.generation == handle.generation ? const_cast<coEntity*>(&entity) : nullptr;
	}
	else
	{
		return nullptr;
	}
}

void coECS::Reserve(coUint32 nbEntities)
{
	coASSERT(false);
}

void coECS::AddProcessor(coProcessor& processor)
{
	coASSERT(!coContains(processors, &processor));
	coPushBack(processors, &processor);
}

coUint32 coECS::GetNbEntities(const coEntityHandle& root) const
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(root);
	if (entity)
	{
		return GetNbEntities(entity->index);
	}
	return 0;
}

coUint32 coECS::GetNbEntities(coUint32 entityIndex) const
{
	coLockScope l(lock);
	const coEntity& entity = _GetEntity(entityIndex);
	coUint32 nb = 1;
	auto countFunc = [&](const coEntity& child)
	{
		nb += GetNbEntities(child.index);
		return true;
	};
	_VisitChildren(entity, countFunc);
	return nb;
}

coEntityHandle coECS::GetFirstChild(const coEntityHandle& handle) const
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity && entity->firstChild != coUint32(-1))
	{
		return _GetEntity(entity->firstChild).GetHandle();
	}
	return coEntityHandle();
}

coEntityHandle coECS::GetPreviousSibling(const coEntityHandle& handle) const
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity && entity->previousSibling != coUint32(-1))
	{
		return _GetEntity(entity->previousSibling).GetHandle();
	}
	return coEntityHandle();
}

coEntityHandle coECS::GetNextSibling(const coEntityHandle& handle) const
{
	coLockScope l(lock);
	coEntity* entity = GetEntity(handle);
	if (entity && entity->nextSibling != coUint32(-1))
	{
		return _GetEntity(entity->nextSibling).GetHandle();
	}
	return coEntityHandle();
}