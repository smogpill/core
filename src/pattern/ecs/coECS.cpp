// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coECS.h"
#include <debug/log/coAssert.h>
#include <container/array/coDynamicArray_f.h>
#include <io/archive/coArchive.h>
#include "entity/coEntityType.h"
#include "storage/coEntityPackStorage.h"
#include "component/coComponentIterator.h"
#include "component/coComponentType.h"

coDEFINE_SINGLETON(coECS);

coECS::~coECS()
{
	// TODO destroy all entities
}

coEntityHandle coECS::CreateEntity(const coType& type)
{
	const coEntityHandle handle = _CreateEmptyEntity();
	coEntity& entity = entities[handle.index];
	entity.entityType = static_cast<const coEntityType*>(type.customTypeData);
	entity.CreateComponents();
	return handle;
}

coEntityHandle coECS::_CreateEmptyEntity()
{
	if (freeEntities.count == 0)
	{
		const coUint32 startIndex = entities.count;
		const coUint32 nbAdded = 256;
		coResize(entities, entities.count + nbAdded);
		coResize(freeEntities, nbAdded);
		for (coUint32 index = 0; index < nbAdded; ++index)
			freeEntities[index] = startIndex + index;
	}
	const coUint32 index = coPopBack(freeEntities);
	coEntity& entity = entities[index];

	// temp
	entity.index = index;

	const coUint32 generation = entity.generation;
	new (&entity) coEntity();
	entity.generation = generation;
	return coEntityHandle(index, entity.generation);
}

coComponent* coECS::GetComponent(const coEntityHandle& entityHandle, const coType& type) const
{
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
	coEntity& entity = entities[index];

	// Destroy children
	auto destroyChild = [&](const coUint32 childIdx)
	{
		DestroyEntity(childIdx);
		return true;
	};
	ReverseVisitChildren(entity, destroyChild);

	entity.SetState(coEntity::State::NONE);
	entity.~coEntity();
	++entity.generation;
	coPushBack(freeEntities, index);
}

coEntityHandle coECS::Clone(const coEntityHandle& entityHandle)
{
	const coEntity* source = GetEntity(entityHandle);
	if (source == nullptr)
		return coEntityHandle();

	coASSERT(source->state == coEntity::State::NONE); // Other states not supported yet

	const coEntityHandle targetHandle = _CreateEmptyEntity();
	coEntity& target = _GetEntity(targetHandle.index);
	target.entityType = source->entityType;
	target.CreateComponents(*source);
	return targetHandle;
}

void coECS::SaveEntity(coArchive& archive, const coEntityHandle& handle) const
{
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
	const coEntity& entity = _GetEntity(entityIndex);
	const coUint32 storageIndex = packStorage.entities.count;
	coEntityStorage& entityStorage = coPushBack(packStorage.entities);
	entityStorage.index = storageIndex;
	entityStorage.parent = parentEntityStorageIndex;
	entityStorage.typeUID = entity.entityType->type->uid;

	coUint32 childIdx = entity.firstChild;
	while (childIdx != coUint32(-1))
	{
		const coEntity& child = _GetEntity(childIdx);
		SaveEntity(packStorage, childIdx, storageIndex);
		childIdx = child.nextSibling;
	}
}

coEntityHandle coECS::LoadEntity(const coArchive& archive)
{
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
	coEntity* child = GetEntity(childHandle);
	coASSERT(child);
	if (child->parent != coUint32(-1))
	{
		if (child->previousSibling != coUint32(-1))
		{
			coEntity& previousSibling = entities[child->previousSibling];
			coASSERT(previousSibling.nextSibling == childHandle.index);
			previousSibling.nextSibling = child->nextSibling;
		}
		if (child->nextSibling != coUint32(-1))
		{
			coEntity& nextSibling = entities[child->nextSibling];
			coASSERT(nextSibling.previousSibling == childHandle.index);
			nextSibling.previousSibling = child->previousSibling;
		}
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
		}
		else
		{
			coEntity& firstChild = entities[parent->firstChild];
			const coUint32 lastChildIdx = firstChild.previousSibling;
			if (lastChildIdx == coUint32(-1))
			{
				firstChild.nextSibling = childHandle.index;
			}
			else
			{
				coEntity& lastChild = entities[lastChildIdx];
				lastChild.nextSibling = childHandle.index;
			}
		}
	}
}

void coECS::SetStarted(const coEntityHandle& handle, coBool b)
{
	coEntity* entity = GetEntity(handle);
	if (entity)
	{
		entity->SetState(b ? coEntity::State::STARTED : coEntity::State::INITIALIZED);
	}
}

coBool coECS::IsAlive(const coEntityHandle& handle) const
{
	return GetEntity(handle) != nullptr;
}

coEntity* coECS::GetEntity(const coEntityHandle& handle) const
{
	if (handle.IsValid())
	{
		const coEntity& entity = entities[handle.index];
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

coUint coECS::GetNbEntities(const coEntityHandle& root) const
{
	coEntity* entity = GetEntity(root);
	if (entity)
	{
		return GetNbEntities(root.index);
	}
	else
	{
		return 0;
	}
}

coUint coECS::GetNbEntities(coUint32 entityIndex) const
{
	const coEntity& entity = _GetEntity(entityIndex);
	coUint nb = 1;
	coUint32 childIdx = entity.firstChild;
	while (childIdx != coUint32(-1))
	{
		nb += GetNbEntities(childIdx);
		const coEntity& child = _GetEntity(childIdx);
		childIdx = child.nextSibling;
	}
	return nb;
}
