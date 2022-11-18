// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coCheckLinks_f.h"
#include "../coDCEL.h"

void coCheckEdgesNotLinkedToDeadEdges(coDCEL& dcel)
{
	auto& edges = dcel.halfEdges;
	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		if (dcel.IsEdgeAlive(edgeIdx))
		{
			const coHalfEdge& edge = edges[edgeIdx];
			coASSERT(dcel.IsEdgeAlive(edge.next));
			coASSERT(dcel.IsEdgeAlive(edge.prev));
			coASSERT(dcel.IsEdgeAlive(edge.twin));
		}
	}
}
