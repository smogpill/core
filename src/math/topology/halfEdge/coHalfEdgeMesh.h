// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "coHalfEdge.h"

class coHalfEdgeMesh
{
public:
	coHalfEdgeMesh() {}
	coHalfEdgeMesh(const coArray<coUint32>& indices, coUint32 nbVertices = coUint32(-1));

	coDynamicArray<coHalfEdge> halfEdges;
};
