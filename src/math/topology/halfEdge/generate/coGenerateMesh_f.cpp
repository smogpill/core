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
	coDynamicArray<coUint32> loopVertices; // temp
	coDynamicArray<coUint32> triangleVertices;
	coTriangulateScratch triangulateScratch;
	coResize(oldVertexToNewVertex, vertexPositions.count, ~coUint32(0));

	// Temp
	struct VToH
	{
		coUint32 newVertexIdx;
		coUint32 edgeIdx;
	};
	coDynamicArray<VToH> vtoh;

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

		// temp: check that the loop does not contain the same vertex more than once
#ifndef coRELEASE
		{
			coClear(loopVertices);
			for (const coHalfEdge* faceEdge : faceEdges)
			{
				coASSERT(!coContains(loopVertices, faceEdge->vertexIdx));
				coPushBack(loopVertices, faceEdge->vertexIdx);
			}
		}
#endif

		// Build the polygon
		{
			coClearAndResize(polygon.vertices, faceEdges.count);
			for (coUint32 idx = 0; idx < faceEdges.count; ++idx)
			{
				const coHalfEdge* faceEdge = faceEdges[idx];
				polygon.vertices[idx] = vertexPositions[faceEdge->vertexIdx];

				// temp
				if (faceEdge->vertexIdx == 12)
				{
					int kk = 0; ++kk;
				}
			}
		}

		// Triangulate
		{
			const coVec3 faceNormal = faceNormals[edge.faceIdx];
			coTriangulateAssumingFlat(polygon, triangleVertices, triangulateScratch, faceNormal);
			coASSERT(triangleVertices.count % 3 == 0);
		}

		// temp
		const coUint32 indexOffset = outIndices.count;

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

			VToH v;
			v.edgeIdx = edges[polyEdge->next].prev;
			v.newVertexIdx = newVertexIdx;
			coPushBack(vtoh, v);
		}

		const coUint32 nbTriangles = triangleVertices.count / 3;
		for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
		{
			const coUint32 triangleOffset = indexOffset + triangleIdx * 3;
			for (coUint32 i = 0; i < 3; ++i)
			{
				const coUint32 j = (i + 1) % 3;
				coUint32 a = outIndices[triangleOffset + i];
				coUint32 b = outIndices[triangleOffset + j];
				if (a > b)
					coSwap(a, b);

				if (a == 5 && b == 7)
				{
					int kk = 0; ++kk;
				}
			}
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