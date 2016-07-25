// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/project/coParsedProject.h"
#include "parser/source/coParsedType.h"
#include "container/array/coDynamicArray_f.h"

coParsedProject::~coParsedProject()
{
	coDeleteElementsAndClear(parsedTypes);
}
