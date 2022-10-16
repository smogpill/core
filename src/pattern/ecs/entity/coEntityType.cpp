// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityType.h"
#include <lang/reflect/coType.h>
#include <container/array/coDynamicArray_f.h>
#include "../component/coComponent.h"

void coEntityType::AddComponent(const coType& type)
{
	for (coType* dependency : type.GetDependencies())
	{
		if (dependency->IsCompatibleWith<coComponent>())
		{
			AddComponent(*dependency);
		}
	}
	coPushBack(componentTypes, &type);
}
