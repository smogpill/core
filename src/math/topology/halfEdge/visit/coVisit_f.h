// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coHalfEdgeMesh.h"

/// Does not work if the vertex is non-manifold (joined by two or more surfaces).
/// There is no specific order, it can be visit in one order, then change halfway.
/// Returns true if all edges were visited
template <class Functor>
coBool coVisitAllHalfEdgesAroundVertex(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, Functor functor)
{
	coDEBUG_CODE(const coUint32 vertexIdx = mesh.halfEdges[halfEdgeIdx].vertexIdx);

	const auto& edges = mesh.halfEdges;

	// First we loop in one direction
	{
		coUint32 itEdgeIdx = halfEdgeIdx;
		for (;;)
		{
			coASSERT(mesh.halfEdges[itEdgeIdx].vertexIdx == vertexIdx);
			const coHalfEdge& edge = edges[itEdgeIdx];
			const coUint32 prevIdx = edge.prev;
			if (!functor(itEdgeIdx))
				return false;
			const coHalfEdge& prev = edges[prevIdx];
			if (prev.twin == prevIdx)
				break;
			itEdgeIdx = prev.twin;

			// Looped?
			if (itEdgeIdx == halfEdgeIdx)
				return true;
		};
	}
	
	// We didn't loop, so we loop in the other direction to visit the remaining
	{
		coUint32 itEdgeIdx = halfEdgeIdx;
		for (;;)
		{
			const coHalfEdge& edge = edges[itEdgeIdx];
			const coUint32 twinIdx = edge.twin;
			if (twinIdx == itEdgeIdx)
				break;
			const coHalfEdge& twin = edges[twinIdx];
			itEdgeIdx = twin.next;
			if (!functor(itEdgeIdx))
				return false;
		}
	}

	return true;
}

template <class Functor>
void coVisitAllHalfEdgesAroundFace(const coHalfEdgeMesh& mesh, coUint32 anyHalfEdgeOfTheFaceIdx, Functor functor)
{
	coUint32 itEdgeIdx = anyHalfEdgeOfTheFaceIdx;
	do
	{
		const coUint32 nextIdx = mesh.halfEdges[itEdgeIdx].next;
		if (!functor(itEdgeIdx))
			return;
		itEdgeIdx = nextIdx;
	} while (itEdgeIdx != anyHalfEdgeOfTheFaceIdx);
}
