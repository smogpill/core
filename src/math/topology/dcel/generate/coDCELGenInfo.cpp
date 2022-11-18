// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDCELGenInfo.h"
#include <container/array/coDynamicArray_f.h>

void coDCELGenInfo::Clear()
{
	coClear(triangleToHalfEdge);
}

void coDCELGenInfo::ShrinkToFit()
{
	coShrinkToFit(triangleToHalfEdge);
}
