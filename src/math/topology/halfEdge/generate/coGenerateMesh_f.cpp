// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coGenerateMesh_f.h"
#include "../coHalfEdgeMesh.h"
#include "../../../vector/coVec3_f.h"
#include "../../../shape/polygon/coPolygon3_f.h"
#include <debug/profiler/coProfile.h>

void coGenerateMesh(const coHalfEdgeMesh& halfEdgeMesh, const coArray<coVec3>& vertexPositions, const coArray<coVec3>& faceNormals, coDynamicArray<coUint32>& outVertices, coDynamicArray<coUint32>& outIndices)
{
	coPROFILE_EVENT();
	coClear(outVertices);
	coClear(outIndices);
	const auto& edges = halfEdgeMesh.halfEdges;
	coPolygon3 polygon;
	coDynamicArray<coUint32> oldVertexToNewVertex;
	coDynamicArray<const coHalfEdge*> faceEdges;
	coDynamicArray<coUint32> triangleVertices;
	coDynamicArray<coBool> doneEdges;
	coTriangulateScratch triangulateScratch;

	coResize(oldVertexToNewVertex, vertexPositions.count, coUint32(-1));
	coResize(doneEdges, edges.count, false);

	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		if (doneEdges[edgeIdx])
			continue;
		const coHalfEdge& edge = edges[edgeIdx];
		if (edge.next == edgeIdx)
			continue;

		// Collect face edges
		{
			coClear(faceEdges);
			coUint32 itEdgeIdx = edgeIdx;
			do
			{
				const coHalfEdge& itNode = edges[itEdgeIdx];
				coPushBack(faceEdges, &itNode);
				itEdgeIdx = itNode.next;
			} while (itEdgeIdx != edgeIdx);
		}

		// Build the polygon
		{
			coReserve(polygon.vertices, faceEdges.count);
			coClear(polygon);
			for (const coHalfEdge* faceEdge : faceEdges)
			{
				coPushBack(polygon.vertices, vertexPositions[faceEdge->vertexIdx]);
			}
		}

		// Triangulate
		{
			const coVec3 faceNormal = faceNormals[edge.faceIdx];
			coTriangulateAssumingFlat(polygon, triangleVertices, triangulateScratch, -faceNormal);
			coASSERT(triangleVertices.count % 3 == 0);
		}

		// Push new vertices and triangles
		for (const coUint32 polyVertexIdx : triangleVertices)
		{
			const coHalfEdge* polyNode = faceEdges[polyVertexIdx];
			coUint32& newVertexIdx = oldVertexToNewVertex[polyNode->vertexIdx];
			if (newVertexIdx == coUint32(-1))
			{
				newVertexIdx = outVertices.count;
				coPushBack(outVertices, polyNode->vertexIdx);
			}
			coPushBack(outIndices, newVertexIdx);
		}

		// Disable the edges of the face
		{
			coUint32 itNodeIdx = edgeIdx;
			do
			{
				const coHalfEdge& itNode = edges[itNodeIdx];
				doneEdges[itNodeIdx] = true;
				itNodeIdx = itNode.next;
			} while (itNodeIdx != edgeIdx);
		}
	}
}