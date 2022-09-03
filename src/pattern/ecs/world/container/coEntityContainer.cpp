// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityContainer.h"
#include "../../entity/coEntityHandle.h"
#include "../../component/coComponent.h"
#include "../../component/coComponentMask.h"
#include "../../component/coComponentTypeHandle.h"
#include "../../component/coComponentRegistry.h"
#include "../coEntityWorld.h"
#include <lang/reflect/coType.h>
#include <memory/coMemory_f.h>

coEntityContainer::coEntityContainer(coEntityWorld& world_, const coComponentMask& mask_)
	: world(world_)
	, componentMask(mask_)
{
	nbComponents = coUint16(mask_.GetNbComponents());
	if (nbComponents)
	{
		componentTypes = new const coType* [nbComponents];
		components = new void*[nbComponents];

		coComponentRegistry* componentRegistry = coComponentRegistry::instance;
		coASSERT(componentRegistry);

		coUint componentIdx = 0;
		auto func = [&](coComponentTypeHandle h)
		{
			coASSERT(componentIdx < nbComponents);
			componentTypes[componentIdx] = componentRegistry->GetType(h);
			components[componentIdx] = nullptr;
			++componentIdx;
		};
		mask_.VisitTypes(func);
		coASSERT(componentIdx == nbComponents);
	}
}

coEntityContainer::~coEntityContainer()
{
	if (buffer)
		coAllocator::GetHeap()->FreeAligned(buffer);
}

void coEntityContainer::Reserve(coUint32 nb)
{
	if (nbReservedEntities >= nb)
		return;

	// Configure new buffer
	coUint64 requiredCapacity8 = sizeof(coEntityHandle) * nb;

	coUint64 componentOffsets[co_maxNbComponentsPerWorld];
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* type = componentTypes[componentIdx];
		requiredCapacity8 = coAlignSize(requiredCapacity8, coUint64(type->alignment8));
		componentOffsets[componentIdx] = requiredCapacity8;
		const coUint64 componentArraySize8 = coUint64(nb) * type->size8;
		requiredCapacity8 += componentArraySize8;
	}

	void* newBuffer = coAllocator::GetHeap()->AllocateAligned(requiredCapacity8, 16);
	coASSERT(newBuffer);

	// Move entities from the old buffer
	if (coLIKELY(nbEntities))
		coMemCopy(newBuffer, buffer, coUint64(nbEntities) * sizeof(coEntityHandle));
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* type = componentTypes[componentIdx];
		void* newComponent = static_cast<coByte*>(newBuffer) + componentOffsets[componentIdx];
		if (coLIKELY(nbEntities))
			coMemCopy(newComponent, components[componentIdx], coUint64(nbEntities) * type->size8);
		components[componentIdx] = static_cast<coComponent*>(newComponent);
	}

	// Free previous buffer
	if (buffer)
		coAllocator::GetHeap()->FreeAligned(buffer);

	buffer = newBuffer;
	entities = static_cast<coEntityHandle*>(newBuffer);
	nbReservedEntities = nb;
}

coComponent& coEntityContainer::GetComponent(coUint32 entityIdx, coUint32 componentTypeIdx) const
{
	coASSERT(componentTypeIdx < nbComponents);
	coASSERT(entityIdx < nbEntities);
	return *reinterpret_cast<coComponent*>(static_cast<coByte*>(components[componentTypeIdx]) + entityIdx * componentTypes[componentTypeIdx]->size8);
}

coComponent* coEntityContainer::FindComponent(coUint32 entityIdx, const coType& type) const
{
	const coUint index = FindComponentTypeIndex(type);
	if (index == coUint(-1))
		return nullptr;
	return &GetComponent(entityIdx, index);
}

coUint coEntityContainer::FindComponentTypeIndex(const coType& type) const
{
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		if (componentTypes[componentIdx] == &type)
		{
			return componentIdx;
		}
	}
	return coUint(-1);
}

coUint32 coEntityContainer::CreateEntity(const coEntityHandle& entity)
{
	const coUint32 index = nbEntities;
	const coUint32 newNbEntities = nbEntities + 1;
	Reserve(newNbEntities);
	nbEntities = newNbEntities;
	entities[index] = entity;
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* type = componentTypes[componentIdx];
		type->constructFunc(static_cast<coByte*>(components[componentIdx]) + index * type->size8);
	}
	return index;
}

void coEntityContainer::DestroyEntity(coUint32 index)
{
	coASSERT(index < nbEntities);
	const coUint32 lastEntityIdx = nbEntities - 1;

	// Copy the last entity
	if (index < lastEntityIdx)
	{
		for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coType* type = componentTypes[componentIdx];
			void* from = components[lastEntityIdx];
			void* to = components[index];
			type->moveFunc(from, to);
			type->destructFunc(from);
		}
		entities[index] = entities[lastEntityIdx];
		world._SetContainerIndex(entities[index], index);
	}
	else
	{
		for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coType* type = componentTypes[componentIdx];
			coComponent* comp = &GetComponent(index, componentIdx);
			type->destructFunc(comp);
		}
	}

	nbEntities = lastEntityIdx;
}

coUint16 coEntityContainer::GetIndexOfComponent(const coComponentTypeHandle& handle) const
{
	coComponentRegistry* componentRegistry = coComponentRegistry::instance;
	coASSERT(componentRegistry);
	const coType* type = componentRegistry->GetType(handle);
	coASSERT(type);
	for (coUint16 componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		if (componentTypes[componentIdx] == type)
			return componentIdx;
	}
	coASSERT(false);
	return coUint16(-1);
}
