// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityContainer.h"
#include "../../entity/coEntityHandle.h"
#include <lang/reflect/coType.h>
#include <memory/coMemory_f.h>

coEntityContainer::~coEntityContainer()
{
	coASSERT(nbEntities == 0);
	if (buffer)
		coAllocator::GetHeap()->FreeAligned(buffer);
}

void coEntityContainer::Reserve(coUint32 nb)
{
	if (nbReservedEntities >= nb)
		return;

	// Configure new buffer
	coUint64 requiredCapacity8 = 0;

	coASSERT(false); // initialize component buffers
	coUint64* componentOffsets = nullptr;
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* type = componentTypes[componentIdx];
		const coUint64 componentArraySize8 = coUint64(nb) * type->size8;
		componentOffsets[componentIdx] = requiredCapacity8;
		requiredCapacity8 += componentArraySize8;
	}

	void* newBuffer = coAllocator::GetHeap()->AllocateAligned(requiredCapacity8, 16);
	coASSERT(newBuffer);

	// Move entities from the old buffer
	for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* type = componentTypes[componentIdx];
		void* newComponent = static_cast<coByte*>(newBuffer) + componentOffsets[componentIdx];
		if (nbEntities)
			coMemCopy(newComponent, components[componentIdx], coUint64(nbEntities) * type->size8);
		components[componentIdx] = static_cast<coComponent*>(newComponent);
	}

	// Free previous buffer
	if (buffer)
		coAllocator::GetHeap()->FreeAligned(buffer);

	buffer = newBuffer;
	nbReservedEntities = nb;
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
		coComponent* comp = components[index];
		type->constructFunc(comp);
	}
	return index;
}

void coEntityContainer::DestroyEntity(coUint32 index)
{
	coASSERT(index < nbEntities);
	--nbEntities;

	// Copy the last entity
	if (index < nbEntities)
	{
		for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coType* type = componentTypes[componentIdx];
			coComponent* comp = components[index];
			type->destructFunc(comp);
		}
	}
	else
	{
		for (coUint componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coType* type = componentTypes[componentIdx];
			coComponent* from = components[nbEntities];
			coComponent* to = components[index];
			type->moveFunc(from, to);
			type->destructFunc(from);
		}
		entities[index] = entities[nbEntities];
	}
}
