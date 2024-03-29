// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coEntityType.h"
#include <lang/reflect/coType.h>
#include <container/array/coDynamicArray_f.h>
#include "../component/coComponent.h"
#include "../component/coComponentType.h"

void coEntityType::AddComponent(const coType& type_)
{
	coASSERT(type_.IsCompatibleWith<coComponent>());
	const coComponentType* componentType = static_cast<const coComponentType*>(type_.customTypeData);
	for (coType* dependency : componentType->GetDependencies())
	{
		if (dependency->IsCompatibleWith<coComponent>())
		{
			AddComponent(*dependency);
		}
	}
	if (!coContains(componentTypes, &type_))
		coPushBack(componentTypes, &type_);
}

coInt coEntityType::GetComponentIndexByTypeUID(coUint32 uid) const
{
	for (coUint idx = 0; idx < componentTypes.count; ++idx)
	{
		const coType* compType = componentTypes[idx];
		if (compType->uid == uid)
			return idx;
	}
	return -1;
}
