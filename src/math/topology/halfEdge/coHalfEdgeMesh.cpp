// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include <container/array/coDynamicArray_f.h>
#include <container/map/coHashMap.h>
#include "coHalfEdgeMesh.h"

coHalfEdgeMesh::coHalfEdgeMesh(const coArray<coUint32>& indices, coUint32 nbVertices)
{
	coASSERT(indices.count % 3 == 0);

	// If no vertex count was provided, just compute a working count by searching for the max vertex index used.
	if (nbVertices == coUint32(-1))
	{
		coUint32 maxIdx = indices.count ? indices[0] : coUint32(-1);
		for (coUint32 ind : indices)
			if (ind > maxIdx)
				maxIdx = ind;
		nbVertices = maxIdx + 1;
	}

	const coUint32 nbTriangles = indices.count / 3;

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

	// Create edge loops
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

	// Link neighboring edge loops
	{
		coUint32 triangleStartIdx = 0;
		for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
		{
			for (coUint32 i = 0; i < 3; ++i)
			{
				const coUint32 indexIdx = triangleStartIdx + i;
				const coUint32 nextIndexIdx = triangleStartIdx + (i + 1) % 3;
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
						nextRadial.prevRadial = entry.halfEdge;
						halfEdge.nextRadial = entry.halfEdge;
					}
					itIdx = entry.next;
				} while (vertexToHalfEdge[itIdx].halfEdge != coUint32(-1));
			}
			triangleStartIdx += 3;
		}
	}
}

void coHalfEdgeMesh::RemoveHalfEdge(coUint32 edgeIdx)
{
	coHalfEdge& edge = halfEdges[edgeIdx];

	// Unlink
	const coUint32 prev = edge.prev;
	const coUint32 prevRadial = edge.prevRadial;
	halfEdges[edge.next].prev = edge.prev;
	halfEdges[prev].next = edge.next;
	halfEdges[edge.nextRadial].prevRadial = edge.prevRadial;
	halfEdges[prevRadial].nextRadial = edge.nextRadial;

	const coUint32 lastEdgeIdx = halfEdges.count - 1;

	// Update edge indices of the last halfEdge of the list
	if (edgeIdx != lastEdgeIdx)
	{
		coHalfEdge& newEdge = halfEdges[lastEdgeIdx];
		newEdge.edgeIdx = edgeIdx;
		const coUint32 newPrev = newEdge.prev;
		const coUint32 newPrevRadial = newEdge.prevRadial;
		halfEdges[newEdge.next].prev = edgeIdx;
		halfEdges[newPrev].next = edgeIdx;
		halfEdges[newEdge.nextRadial].prevRadial = edgeIdx;
		halfEdges[newPrevRadial].nextRadial = edgeIdx;
	}

	coRemoveUnorderedByIndex(halfEdges, edgeIdx);
}

coUint32 coHalfEdgeMesh::GetNbFaces() const
{
	coUint32 nb = 0;
	auto functor = [&](const coHalfEdge& edge)
	{
		++nb;
		return true;
	};
	VisitFaces(functor);
	return nb;
}

coUint32 coHalfEdgeMesh::GetNbNonDegenerateFaces() const
{
	coUint32 nb = 0;
	auto functor = [&](const coHalfEdge& edge)
	{
		if (edge.next != edge.prev)
			++nb;
		return true;
	};
	VisitFaces(functor);
	return nb;
}

coUint32 coHalfEdgeMesh::GetNbDegenerateFaces() const
{
	coUint32 nb = 0;
	auto functor = [&](const coHalfEdge& edge)
	{
		if (edge.next == edge.prev)
			++nb;
		return true;
	};
	VisitFaces(functor);
	return nb;
}
