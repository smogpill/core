// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coRemoveUnused_f.h"
#include "../coHalfEdgeMesh.h"
#include <container/array/coDynamicArray_f.h>

void coRemoveUnusedVertices(coHalfEdgeMesh& mesh)
{
	coDynamicArray<coVec3> newVertices;
	const coArray<coVec3>& oldVertices = mesh.vertices;
	coDynamicArray<coUint32> oldVertexToNew;
	auto& halfEdges = mesh.halfEdges;
	coReserve(newVertices, oldVertices.count);
	coResize(oldVertexToNew, oldVertices.count, ~coUint32(0));
	for (coUint32 halfEdgeIdx = 0; halfEdgeIdx < halfEdges.count; ++halfEdgeIdx)
	{
		coHalfEdge& halfEdge = halfEdges[halfEdgeIdx];
		if (halfEdge.next == halfEdgeIdx)
		{
			halfEdge.vertexIdx = ~coUint32(0);
		}
		else
		{
			coUint32& newIdx = oldVertexToNew[halfEdge.vertexIdx];
			if (newIdx == ~coUint32(0))
			{
				newIdx = newVertices.count;
				coPushBack(newVertices, oldVertices[halfEdge.vertexIdx]);
			}
			halfEdge.vertexIdx = newIdx;
		}
	}
	mesh.vertices = std::move(newVertices);
}
