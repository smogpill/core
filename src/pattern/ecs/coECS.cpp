// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coECS.h"
#include <debug/log/coAssert.h>
#include "entity/coEntityType.h"
#include "component/coComponentIterator.h"
#include "component/coComponentType.h"

coDEFINE_SINGLETON(coECS);

coECS::~coECS()
{
	// TODO destroy all entities
}

coEntityHandle coECS::CreateEntity(const coEntityType& entityType)
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
	const coUint32 generation = entity.generation;
	new (&entity) coEntity();
	entity.generation = generation;
	entity.entityType = &entityType;
	entity.CreateComponents();

	// Create components
	for (const coType* type : entityType.GetComponentTypes())
	{
		coComponent* component = static_cast<coComponent*>(type->createFunc());
		const coComponentTypeData* componentType = static_cast<const coComponentTypeData*>(type->customTypeData);
		if (componentType->initFunc)
			componentType->initFunc(*component);
		entity.Give(*component);
	}

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

	// Delete components
	{
		const coEntityType* entityType = entity.entityType;
		const auto& componentTypes = entityType->GetComponentTypes();
		coUint32 componentIdx = componentTypes.count - 1;
		auto func = [&](coComponent& comp)
		{
			const coType* componentType = componentTypes[componentIdx];
			const coComponentTypeData* componentTypeData = static_cast<const coComponentTypeData*>(componentType->customTypeData);
			if (componentTypeData->shutdownFunc)
				componentTypeData->shutdownFunc(*comp);
			delete& comp;
			return true;
		};
		coReverseVisitAll(entity.GetFirstComponent(), func);
	}

	entity.~coEntity();
	++entity.generation;
	coPushBack(freeEntities, index);
}

coEntityHandle coECS::Clone(const coEntityHandle& entityHandle)
{
	coASSERT(false);
	return coEntityHandle();
}

void coECS::SaveEntity(coArchive& archive, const coEntityHandle&) const
{
	coASSERT(false);
}

coEntityHandle coECS::LoadEntity(const coArchive& archive)
{
	coASSERT(false);
	return coEntityHandle();
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
	child->parent = parentHandle.index;
	if (parentHandle.IsValid())
	{
		coEntity* parent = GetEntity(parentHandle);
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
		if (entity->startedRequested == b)
			return;
		entity->startedRequested = b;
		const coArray<const coType*>& componentTypes = entity->entityType->GetComponentTypes();

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
