// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDissolveDegenerateFaces_f.h"
#include <container/array/coArray_f.h>
#include "../coHalfEdgeMesh.h"

void coDissolveDegenerateFace(coHalfEdgeMesh& mesh, coUint32 anyHalfEdgeIdx)
{
	auto& halfEdges = mesh.halfEdges;
	coHalfEdge& edge = halfEdges[anyHalfEdgeIdx];
	coASSERT(edge.IsDegenerate());
	const coUint32 nextIdx = edge.next;
	coHalfEdge& next = halfEdges[nextIdx];

	halfEdges[edge.nextRadial].nextRadial = next.nextRadial;
	halfEdges[next.nextRadial].nextRadial = edge.nextRadial;

	edge.next = anyHalfEdgeIdx;
	edge.prev = anyHalfEdgeIdx;
	edge.nextRadial = anyHalfEdgeIdx;
	edge.prevRadial = anyHalfEdgeIdx;

	next.next = nextIdx;
	next.prev = nextIdx;
	next.nextRadial = nextIdx;
	next.prevRadial = nextIdx;
}

void coRemoveDegenerateFaces(coHalfEdgeMesh& mesh)
{
	auto& halfEdges = mesh.halfEdges;
	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count;)
	{
		coHalfEdge& edge = halfEdges[edgeIdx];
		if (edge.next == edge.prev)
		{
			mesh.RemoveHalfEdge(edgeIdx);
			continue;
		}
		++edgeIdx;
	}
}