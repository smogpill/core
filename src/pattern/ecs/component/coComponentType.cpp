// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponentType.h"
#include "coComponent.h"
#include <container/array/coDynamicArray_f.h>

void coComponentTypeData::AddDependency(const coType& type)
{
	coASSERT(type.IsCompatibleWith<coComponent>());
	coASSERT(!coContains(dependencies, const_cast<coType*>(&type)));
	coPushBack(dependencies, const_cast<coType*>(&type));
}