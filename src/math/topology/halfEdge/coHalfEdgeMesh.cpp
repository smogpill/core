// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include <container/array/coDynamicArray_f.h>
#include <container/map/coHashMap.h>
#include "coHalfEdgeMesh.h"

coHalfEdgeMesh::coHalfEdgeMesh(const coArray<coUint32>& indices, const coUint32 nbVertices)
{
	coASSERT(indices.count % 3 == 0);
	const coUint32 nbTriangles = indices.count / 3;

	/*
	struct Edge
	{
		coUint32 verts[2];
		coUint32 halfEdge;
		coUint32 next;
	};

	coHashMap<coUint64, coUint32, 1024, [](const coUint64 key){return }>
	coHash
	coUint32 edgeBuckets[1024];
	for (coUint32& bucket : edgeBuckets)
	{
		bucket = coUint32(-1);
	}

	auto computeEdgeKey = [](const coUint32 a, const coUint32 b)
	{

	};

	*/

	struct VertexToHalfEdge
	{
		coUint32 halfEdge = coUint32(-1);
		coUint32 next = coUint32(-1);
	};

	coDynamicArray<VertexToHalfEdge> vertexToHalfEdge;
	coResize(vertexToHalfEdge, nbVertices);

	auto pushBackVertxToHalfEdge = [&](const coUint32 vertex, const coUint32 halfEdge)
	{
		coUint32 index = vertex;
		while (vertexToHalfEdge[index].halfEdge != coUint32(-1))
			index = vertexToHalfEdge[index].next;

		VertexToHalfEdge& entry = vertexToHalfEdge[index];
		entry.halfEdge = halfEdge;
		entry.next = vertexToHalfEdge.count;
		coPushBack(vertexToHalfEdge, VertexToHalfEdge());
	};

	coReserve(halfEdges, nbTriangles * 3);
	for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
	{
		const coUint32 nodeOffset = halfEdges.count;
		for (coUint32 i = 0; i < 3; ++i)
		{
			const coUint32 edgeIdx = halfEdges.count;
			const coUint32 indexIdx = triangleIdx * 3 + i;
			const coUint32 vertexIdx = indices[indexIdx];
			pushBackVertxToHalfEdge(vertexIdx, halfEdges.count);
			coHalfEdge& edge = coPushBack(halfEdges);
			edge.vertexIdx = vertexIdx;
			edge.edgeIdx = indexIdx;
			edge.faceIdx = triangleIdx;
			edge.prev = nodeOffset + (i + 2) % 3;
			edge.next = nodeOffset + (i + 1) % 3;
			edge.prevRadial = edgeIdx;
			edge.nextRadial = edgeIdx;
		}
	}

	{
		coUint32 indexIdx = 0;
		for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
		{
			for (coUint32 i = 0; i < 3; ++i)
			{
				const coUint32 nextIndexIdx = (indexIdx + 1) % 3;
				coHalfEdge& halfEdge = halfEdges[indexIdx];
				const coUint32 vertexIdx = indices[indexIdx];
				const coUint32 nextVertexIdx = indices[nextIndexIdx];

				coUint32 itIdx = nextVertexIdx;
				do
				{
					VertexToHalfEdge& entry = vertexToHalfEdge[itIdx];
					const coHalfEdge& itHalfEdge = halfEdges[entry.halfEdge];
					const coHalfEdge& nextHalfEdge = halfEdges[itHalfEdge.next];
					if (nextHalfEdge.vertexIdx == vertexIdx)
					{
						coHalfEdge& nextRadial = halfEdges[halfEdge.nextRadial];
						halfEdge.nextRadial = itIdx;
					}
					itIdx = entry.next;
				} while (vertexToHalfEdge[itIdx].halfEdge != coUint32(-1));
				++indexIdx;
			}
		}
	}
}
