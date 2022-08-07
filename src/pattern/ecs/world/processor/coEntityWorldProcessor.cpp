// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityWorldProcessor.h"
#include <container/array/coDynamicArray_f.h>
#include "../../component/coComponentMask.h"
#include "../../processor/coEntityProcessor.h"

void coEntityWorldProcessor::RegisterEntityType(const coEntityTypeID& entityTypeID, const coComponentMask& mask)
{
	coASSERT(processor);
	coASSERT(!HasEntityType(entityTypeID));
	const coUint nbComponentTypes = processor->GetNbComponentTypes();

	// Check if the entity type has all the required components
	for (coUint componentTypeIdx = 0; componentTypeIdx < nbComponentTypes; ++componentTypeIdx)
	{
		const coComponentTypeHandle handle = componentTypes[componentTypeIdx];
		if (!mask.HasType(handle))
			return;
	}

	EntityTypeInfo& info = coPushBack(entityTypeInfos);
	info.entityTypeID = entityTypeID;
	for (coUint componentTypeIdx = 0; componentTypeIdx < nbComponentTypes; ++componentTypeIdx)
	{
		const coComponentTypeHandle handle = componentTypes[componentTypeIdx];
	}
}

coBool coEntityWorldProcessor::HasEntityType(const coEntityTypeID& entityTypeID) const
{
	for (const EntityTypeInfo& info : entityTypeInfos)
	{
		if (info.entityTypeID == entityTypeID)
			return true;
	}
	return false;
}