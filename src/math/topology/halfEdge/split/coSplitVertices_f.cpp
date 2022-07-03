// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSplitVertices_f.h"
#include "../visit/coVisit_f.h"
#include <container/array/coDynamicArray_f.h>

void coSplitVerticesLinkedToManyFaceFans(coHalfEdgeMesh& mesh)
{
	auto& edges = mesh.halfEdges;
	auto& vertices = mesh.vertices;

	// Reset temporary flags
	for (coHalfEdge& edge : edges)
		edge.done = false;

	coDynamicArray<coBool> touchedVertices;
	coResize(touchedVertices, vertices.count, false);

	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = edges[edgeIdx];
		if (edge.done)
			continue;
		edge.done = true;

		// Vertex already has a fan?
		if (touchedVertices[edge.vertexIdx])
		{
			// Clone the vertex
			const coVec3 vert = vertices[edge.vertexIdx];
			const coUint32 newVertexIdx = vertices.count;
			coPushBack(vertices, vert);

			// Modify the current fan
			auto modifyEdge = [&](const coUint32 itEdgeIdx)
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];
				itEdge.vertexIdx = newVertexIdx;
				itEdge.done = true;
				return true;
			};
			coVisitAllHalfEdgesAroundVertex(mesh, edgeIdx, modifyEdge);
		}
		else
		{
			auto markEdgeAsDone = [&](const coUint32 itEdgeIdx)
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];
				itEdge.done = true;
				return true;
			};
			coVisitAllHalfEdgesAroundVertex(mesh, edgeIdx, markEdgeAsDone);
			touchedVertices[edge.vertexIdx] = true;
		}
	}
}
