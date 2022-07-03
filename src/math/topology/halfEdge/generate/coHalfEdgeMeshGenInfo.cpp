// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coHalfEdgeMeshGenInfo.h"
#include <container/array/coDynamicArray_f.h>

void coHalfEdgeMeshGenInfo::Clear()
{
	coClear(triangleToHalfEdge);
}

void coHalfEdgeMeshGenInfo::ShrinkToFit()
{
	coShrinkToFit(triangleToHalfEdge);
}
