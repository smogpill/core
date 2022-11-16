// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coComponentType.h"
#include "coComponent.h"
#include <container/array/coDynamicArray_f.h>

void coComponentType::AddDependency(const coType& dep)
{
	coASSERT(dep.IsCompatibleWith<coComponent>());
	if (!coContains(dependencies, const_cast<coType*>(&dep)))
		coPushBack(dependencies, const_cast<coType*>(&dep));
}