// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coCheckLinks_f.h"
#include "../coHalfEdgeMesh.h"

void coCheckEdgesNotLinkedToDeadEdges(coHalfEdgeMesh& mesh)
{
	auto& edges = mesh.halfEdges;
	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		if (mesh.IsEdgeAlive(edgeIdx))
		{
			const coHalfEdge& edge = edges[edgeIdx];
			coASSERT(mesh.IsEdgeAlive(edge.next));
			coASSERT(mesh.IsEdgeAlive(edge.prev));
			coASSERT(mesh.IsEdgeAlive(edge.twin));
		}
	}
}
