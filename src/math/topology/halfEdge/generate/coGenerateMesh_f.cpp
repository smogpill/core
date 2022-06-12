// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coGenerateMesh_f.h"
#include "../coHalfEdgeMesh.h"
#include "../../../vector/coVec3_f.h"
#include "../../../shape/polygon/coPolygon3_f.h"
#include <debug/profiler/coProfile.h>

void coGenerateMesh(coHalfEdgeMesh& halfEdgeMesh, const coArray<coVec3>& vertexPositions, const coArray<coVec3>& faceNormals, coDynamicArray<coUint32>& outVertices, coDynamicArray<coUint32>& outIndices)
{
	coPROFILE_EVENT();
	coClear(outVertices);
	coClear(outIndices);
	auto& edges = halfEdgeMesh.halfEdges;
	coPolygon3 polygon;
	coDynamicArray<coUint32> oldVertexToNewVertex;
	coDynamicArray<coHalfEdge*> faceEdges;
	coDynamicArray<coUint32> triangleVertices;
	coTriangulateScratch triangulateScratch;
	coResize(oldVertexToNewVertex, vertexPositions.count, ~coUint32(0));

	/// Clear the 'done' flag
	for (coHalfEdge& edge : edges)
		edge.done = false;

	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = edges[edgeIdx];
		if (edge.done || edge.next == edgeIdx)
			continue;

		// Collect face edges
		{
			coClear(faceEdges);
			coUint32 itEdgeIdx = edgeIdx;
			do
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];
				coPushBack(faceEdges, &itEdge);
				itEdgeIdx = itEdge.next;
			} while (itEdgeIdx != edgeIdx);
		}

		// Build the polygon
		{
			coClearAndResize(polygon.vertices, faceEdges.count);
			for (coUint32 idx = 0; idx < faceEdges.count; ++idx)
			{
				const coHalfEdge* faceEdge = faceEdges[idx];
				polygon.vertices[idx] = vertexPositions[faceEdge->vertexIdx];
			}
		}

		// Triangulate
		{
			const coVec3 faceNormal = faceNormals[edge.faceIdx];
			coTriangulateAssumingFlat(polygon, triangleVertices, triangulateScratch, faceNormal);
			coASSERT(triangleVertices.count % 3 == 0);
		}

		// Push new vertices and triangles
		for (const coUint32 polyVertexIdx : triangleVertices)
		{
			const coHalfEdge* polyEdge = faceEdges[polyVertexIdx];
			coUint32& newVertexIdx = oldVertexToNewVertex[polyEdge->vertexIdx];
			if (newVertexIdx == ~coUint32(0))
			{
				newVertexIdx = outVertices.count;
				coPushBack(outVertices, polyEdge->vertexIdx);
			}
			coPushBack(outIndices, newVertexIdx);
		}

		// Disable the edges of the face
		{
			coUint32 itEdgeIdx = edgeIdx;
			do
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];
				itEdge.done = true;
				itEdgeIdx = itEdge.next;
			} while (itEdgeIdx != edgeIdx);
		}
	}
}