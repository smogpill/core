// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityWorldProcessor.h"
#include <container/array/coDynamicArray_f.h>
#include "../container/coEntityContainer.h"
#include "../../component/coComponentMask.h"
#include "../../processor/coEntityProcessor.h"

void coEntityWorldProcessor::RegisterEntityType(const coEntityTypeID& entityTypeID, const coEntityContainer& container, const coComponentMask& mask)
{
	coASSERT(processor);
	coASSERT(!HasEntityType(entityTypeID));
	const coUint nbComponentTypes = processor->GetNbComponentTypes();

	if (!processor->IsCompatible(mask))
		return;

	EntityTypeInfo& info = coPushBack(entityTypeInfos);
	info.entityTypeID = entityTypeID;
	const coComponentTypeHandle* componentTypeHandles = processor->GetComponentTypeArray();
	coASSERT(componentTypeHandles);
	for (coUint componentTypeIdx = 0; componentTypeIdx < nbComponentTypes; ++componentTypeIdx)
	{
		const coComponentTypeHandle handle = componentTypeHandles[componentTypeIdx];
		info.componentIndicesInEntityType[componentTypeIdx] = container.GetIndexOfComponent(handle);
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