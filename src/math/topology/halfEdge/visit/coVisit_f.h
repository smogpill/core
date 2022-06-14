// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coHalfEdgeMesh.h"

/// Does not work if the vertex is non-manifold (joined by two or more surfaces).
/// The direction of the rotation is the same as the edge (CCW if the edge was CCW).
template <class Functor>
void coVisitAllHalfEdgesAroundVertex(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, Functor functor)
{
	coDEBUG_CODE(const coUint32 vertexIdx = mesh.halfEdges[halfEdgeIdx].vertexIdx);
	coUint32 itEdgeIdx = halfEdgeIdx;
	do
	{
		coASSERT(mesh.halfEdges[itEdgeIdx].vertexIdx == vertexIdx);
		const coHalfEdge& edge = mesh.halfEdges[itEdgeIdx];
		const coUint32 prevIdx = edge.prev;
		functor(itEdgeIdx);
		const coHalfEdge& prev = mesh.halfEdges[prevIdx];
		if (prev.nextRadial == itEdgeIdx)
			break;
		itEdgeIdx = prev.nextRadial;
	} while (itEdgeIdx != halfEdgeIdx);
}
