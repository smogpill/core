// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDissolveDegenerateFaces_f.h"
#include <container/array/coArray_f.h>
#include "../coDCEL.h"

void coDissolveDegenerateFace(coDCEL& dcel, coUint32 anyHalfEdgeIdx)
{
	auto& halfEdges = dcel.halfEdges;
	coHalfEdge& edge = halfEdges[anyHalfEdgeIdx];
	coASSERT(edge.IsDegenerate());
	const coUint32 nextIdx = edge.next;
	coHalfEdge& next = halfEdges[nextIdx];

	halfEdges[edge.twin].twin = next.twin;
	halfEdges[next.twin].twin = edge.twin;

	edge.next = anyHalfEdgeIdx;
	edge.prev = anyHalfEdgeIdx;
	edge.twin = anyHalfEdgeIdx;

	next.next = nextIdx;
	next.prev = nextIdx;
	next.twin = nextIdx;
}

void coRemoveDegenerateFaces(coDCEL& dcel)
{
	auto& halfEdges = dcel.halfEdges;
	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count;)
	{
		coHalfEdge& edge = halfEdges[edgeIdx];
		if (edge.next == edge.prev)
		{
			dcel.RemoveHalfEdge(edgeIdx);
			continue;
		}
		++edgeIdx;
	}
}