// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityTypeRegistry.h"
#include "coEntityType.h"
#include <container/array/coDynamicArray_f.h>

void coEntityTypeRegistry::Add(coEntityType& type)
{
	coASSERT(!coContains(entityTypes, &type));
	coPushBack(entityTypes, &type);
}

coEntityType* coEntityTypeRegistry::Get(coUint32 uid) const
{
	for (coEntityType* entityType : entityTypes)
	{
		if (entityType->type->uid == uid)
		{
			return entityType;
		}
	}
	return nullptr;
}
