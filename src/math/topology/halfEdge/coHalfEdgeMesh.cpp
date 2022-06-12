// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coHalfEdgeMesh.h"
#include <container/array/coDynamicArray_f.h>
#include <container/map/coHashMap_f.h>
#include <debug/profiler/coProfile.h>

coBool co_advancedChecks = true;

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

	coDEBUG_CODE(Check());
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
		const coUint32 newPrev = newEdge.prev;
		const coUint32 newPrevRadial = newEdge.prevRadial;
		halfEdges[newEdge.next].prev = edgeIdx;
		halfEdges[newPrev].next = edgeIdx;
		halfEdges[newEdge.nextRadial].prevRadial = edgeIdx;
		halfEdges[newPrevRadial].nextRadial = edgeIdx;
	}

	coRemoveUnorderedByIndex(halfEdges, edgeIdx);
}

void coHalfEdgeMesh::CheckEdgeLoop(coUint32 edgeIdx) const
{
	if (!co_advancedChecks)
		return;
	const coHalfEdge& edge = halfEdges[edgeIdx];
	edge.checked = true;
	if (!edge.IsDegenerate())
	{
		coUint32 itEdgeIdx = edgeIdx;
		const coUint32 faceIdx = edge.faceIdx;
		do
		{
			const coHalfEdge& itEdge = halfEdges[itEdgeIdx];
			coASSERT(!itEdge.IsDegenerate());
			coASSERT(itEdge.faceIdx == faceIdx);
			CheckEdge(itEdgeIdx);
			itEdgeIdx = itEdge.next;
		} while (itEdgeIdx != edgeIdx);
	}
}

void coHalfEdgeMesh::CheckEdge(coUint32 edgeIdx) const
{
	if (!co_advancedChecks)
		return;
	const coHalfEdge& edge = halfEdges[edgeIdx];
	const coHalfEdge& next = halfEdges[edge.next];
	const coHalfEdge& prev = halfEdges[edge.prev];
	const coHalfEdge& nextRadial = halfEdges[edge.nextRadial];
	const coHalfEdge& prevRadial = halfEdges[edge.prevRadial];
	coASSERT(next.prev == edgeIdx);
	coASSERT(prev.next == edgeIdx);
	coASSERT(nextRadial.prevRadial == edgeIdx);
	coASSERT(prevRadial.nextRadial == edgeIdx);
	edge.checked = true;
}

void coHalfEdgeMesh::CheckNoMoreThan2FacesPerEdge() const
{
	if (!co_advancedChecks)
		return;

	coHashMap<coUint64, coUint8, 1024> edgeToCount;
	coReserve(edgeToCount, halfEdges.count);
	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = halfEdges[edgeIdx];
		if (edge.next == edgeIdx)
			continue;
		const coHalfEdge& nextEdge = halfEdges[edge.next];
		coUint32 aIdx = edge.vertexIdx;
		coUint32 bIdx = nextEdge.vertexIdx;
		if (bIdx < aIdx)
			coSwap(aIdx, bIdx);
		const coUint64 edgeKey = coUint64(aIdx) | (coUint64(bIdx) << 32);
		const coUint8 nb = coGet(edgeToCount, edgeKey, coUint8(0));
		coASSERT(nb < 2);
		coSet(edgeToCount, edgeKey, coUint8(nb + 1));
	}
}

coBool coHalfEdgeMesh::IsEdgeManifold(coUint32 edgeIdx) const
{
	const coHalfEdge& e = halfEdges[edgeIdx];
	return halfEdges[e.nextRadial].nextRadial == edgeIdx;
}

coBool coHalfEdgeMesh::IsEdgeContiguous(coUint32 edgeIdx) const
{
	const coHalfEdge& e = halfEdges[edgeIdx];
	const coHalfEdge& radial = halfEdges[e.nextRadial];
	return halfEdges[e.nextRadial].nextRadial == edgeIdx && (e.vertexIdx == ~coUint32(0) || radial.vertexIdx != e.vertexIdx);
}

void coHalfEdgeMesh::Check() const
{
	if (!co_advancedChecks)
		return;
	
	ClearCheckedFlags();
	for (coUint32 idx = 0; idx < halfEdges.count; ++idx)
	{
		if (!halfEdges[idx].checked)
			CheckEdgeLoop(idx);
	}
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

void coHalfEdgeMesh::ClearCheckedFlags() const
{
	for (const coHalfEdge& edge : halfEdges)
		edge.checked = false;
}

coUint32 coHalfEdgeMesh::AddFace(coUint32 faceIdx, coUint32 nbHalfEdges)
{
	const coUint32 offset = halfEdges.count;

	coReserve(halfEdges, halfEdges.count + nbHalfEdges);
	for (coUint32 idx = 0; idx < nbHalfEdges; ++idx)
	{
		const coUint32 edgeIdx = offset + idx;
		coHalfEdge e;
		e.prev = offset + (idx == 0 ? (nbHalfEdges - 1) : (idx - 1));
		e.next = offset + ((idx + 1) % nbHalfEdges);
		e.nextRadial = edgeIdx;
		e.prevRadial = edgeIdx;
		e.faceIdx = faceIdx;
		coPushBack(halfEdges, e);
	}
	return offset;
}

void coHalfEdgeMesh::SetRadials(coUint32 edgeAIdx, coUint32 edgeBIdx)
{
	coHalfEdge& edgeA = halfEdges[edgeAIdx];
	coHalfEdge& edgeB = halfEdges[edgeBIdx];
	coASSERT(edgeA.nextRadial == edgeAIdx);
	coASSERT(edgeB.nextRadial == edgeBIdx);
	edgeA.nextRadial = edgeBIdx;
	edgeA.prevRadial = edgeBIdx;
	edgeB.nextRadial = edgeAIdx;
	edgeB.prevRadial = edgeAIdx;
}