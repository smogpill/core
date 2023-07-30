// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDCEL.h"
#include "visit/coVisit_f.h"
#include <container/array/coDynamicArray_f.h>
#include <container/map/coHashMap_f.h>
#include <debug/profiler/coProfile.h>

coBool co_advancedChecks = false;

coDCEL::coDCEL(const coArray<coUint32>& indices, const coArray<coVec3>& vertices_)
	: coDCEL(indices, vertices_.count)
{
	vertices = vertices_;
}

coDCEL::coDCEL(const coArray<coUint32>& indices, coUint32 nbVertices)
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

	auto pushBackVertexToHalfEdge = [&](const coUint32 vertex, const coUint32 halfEdge)
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
			pushBackVertexToHalfEdge(vertexIdx, halfEdges.count);
			coHalfEdge edge;
			edge.vertexIdx = vertexIdx;
			edge.faceIdx = triangleIdx;
			edge.prev = nodeOffset + (i + 2) % 3;
			edge.next = nodeOffset + (i + 1) % 3;
			coPushBack(halfEdges, edge);
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
					coHalfEdge& itHalfEdge = halfEdges[entry.halfEdge];
					const coHalfEdge& nextHalfEdge = halfEdges[itHalfEdge.next];
					if (nextHalfEdge.vertexIdx == vertexIdx)
					{
						itHalfEdge.twin = indexIdx;
						halfEdge.twin = entry.halfEdge;
						break;
					}
					itIdx = entry.next;
				} while (vertexToHalfEdge[itIdx].halfEdge != coUint32(-1));
			}
			triangleStartIdx += 3;
		}
	}

	coDEBUG_CODE(Check());
}

void coDCEL::Clear()
{
	coClear(vertices);
	coClear(faceNormals);
	coClear(halfEdges);
}

void coDCEL::ShrinkToFit()
{
	coShrinkToFit(vertices);
	coShrinkToFit(faceNormals);
	coShrinkToFit(halfEdges);
}

void coDCEL::RemoveHalfEdge(coUint32 edgeIdx)
{
	coHalfEdge& edge = halfEdges[edgeIdx];

	// Unlink
	halfEdges[edge.next].prev = edge.prev;
	halfEdges[edge.prev].next = edge.next;
	if (edge.HasTwin())
		halfEdges[edge.twin].twin = coUint32(-1);

	// Update edge indices of the last halfEdge of the list
	const coUint32 lastEdgeIdx = halfEdges.count - 1;
	if (edgeIdx != lastEdgeIdx)
	{
		coHalfEdge& newEdge = halfEdges[lastEdgeIdx];
		const coUint32 newEdgePrev = newEdge.prev; // It can be changed during the upgrade if the face is degenerate
		halfEdges[newEdge.next].prev = edgeIdx;
		halfEdges[newEdgePrev].next = edgeIdx;
		if (newEdge.HasTwin())
			halfEdges[newEdge.twin].twin = coUint32(-1);
	}

	coRemoveUnorderedByIndex(halfEdges, edgeIdx);
}

/// Tests:
/// - At most one face fan per vertex
/// - Circling edges share the same vertex
/// - Edges are contiguous
void coDCEL::CheckManifoldExceptHoles() const
{
	if (!co_advancedChecks)
		return;
	coDynamicArray<coBool> touchedVertices;
	coResize(touchedVertices, vertices.count, false);

	for (const coHalfEdge& edge : halfEdges)
		edge.checked = false;

	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = halfEdges[edgeIdx];
		if (!IsEdgeAlive(edgeIdx))
			continue;

		coASSERT(!edge.HasTwin() || IsEdgeContiguous(edgeIdx));

		if (edge.checked)
			continue;

		const coUint32 vertexIdx = edge.vertexIdx;
		coASSERT(!touchedVertices[vertexIdx]); // More than one face fan for that vertex
		touchedVertices[vertexIdx] = true;

		// Check and mark the face fan circling around that vertex
		auto func = [&](const coUint32 edgeIdx)
		{
			const coHalfEdge& itEdge = halfEdges[edgeIdx];
			coASSERT(!itEdge.checked);
			coASSERT(itEdge.vertexIdx == vertexIdx);
			itEdge.checked = true;
			return true;
		};
		coVisitHalfEdgeFanAroundVertex(*this, edgeIdx, func);
	}
}

void coDCEL::CheckEdgeLoop(coUint32 edgeIdx) const
{
	if (!co_advancedChecks)
		return;
	if (edgeIdx == coUint32(-1))
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

void coDCEL::CheckEdge(coUint32 edgeIdx) const
{
	if (!co_advancedChecks)
		return;
	if (edgeIdx == coUint32(-1))
		return;
	const coHalfEdge& edge = halfEdges[edgeIdx];
	const coHalfEdge& next = halfEdges[edge.next];
	const coHalfEdge& prev = halfEdges[edge.prev];
	coASSERT(next.prev == edgeIdx);
	coASSERT(prev.next == edgeIdx);
	if (edge.HasTwin())
		coASSERT(halfEdges[edge.twin].twin == edgeIdx);
	edge.checked = true;
}

void coDCEL::CheckEdgeNotReferencedByOthers(coUint32 edgeIdx) const
{
	if (!co_advancedChecks)
		return;
	if (edgeIdx == coUint32(-1))
		return;
	for (coUint32 itEdgeIdx = 0; itEdgeIdx < halfEdges.count; ++itEdgeIdx)
	{
		if (itEdgeIdx == edgeIdx) [[unlikely]]
			continue;

		const coHalfEdge& edge = halfEdges[itEdgeIdx];
		coASSERT(edge.next != edgeIdx);
		coASSERT(edge.prev != edgeIdx);
		coASSERT(edge.twin != edgeIdx);
	}
}

void coDCEL::CheckNoMoreThan2FacesPerEdge() const
{
	if (!co_advancedChecks)
		return;

	coHashMap<coUint64, coUint32, 1024> edgeToCount;
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
		const coUint32 foundHalfEdgeIdx = coGet(edgeToCount, edgeKey, coUint32(-1));
		coASSERT(foundHalfEdgeIdx == coUint32(-1) || edge.twin == foundHalfEdgeIdx);
		coSet(edgeToCount, edgeKey, edgeIdx);
	}
}

void coDCEL::CheckNoVertexDuplicatesOnFaces() const
{
	if (!co_advancedChecks)
		return;

	for (const coHalfEdge& edge : halfEdges)
		edge.checked = false;

	coDynamicArray<coBool> touchedVertices;
	coResize(touchedVertices, halfEdges.count, false);
	
	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = halfEdges[edgeIdx];
		if (edge.checked)
			continue;
		if (edge.next == edgeIdx)
			continue;
		auto doEdge = [&](const coUint32 itEdgeidx)
		{
			const coHalfEdge& itEdge = halfEdges[itEdgeidx];
			itEdge.checked = true;
			coBool& touched = touchedVertices[itEdge.vertexIdx];
			coASSERT(!touched);
			touched = true;
			return true;
		};
		coVisitAllHalfEdgesAroundFace(*this, edgeIdx, doEdge);
		auto clearEdge = [&](const coUint32 itEdgeIdx)
		{
			touchedVertices[halfEdges[itEdgeIdx].vertexIdx] = false;
			return true;
		};
		coVisitAllHalfEdgesAroundFace(*this, edgeIdx, clearEdge);
	}
}

coBool coDCEL::IsEdgeManifold(coUint32 edgeIdx) const
{
	const coHalfEdge& e = halfEdges[edgeIdx];
	return e.IsBorder() || halfEdges[e.twin].twin == edgeIdx;
}

coBool coDCEL::IsEdgeContiguous(coUint32 edgeIdx) const
{
	const coHalfEdge& e = halfEdges[edgeIdx];
	if (e.HasTwin())
	{
		const coHalfEdge& twin = halfEdges[e.twin];
		return twin.twin == edgeIdx && (e.vertexIdx == coUint32(-1) || twin.vertexIdx != e.vertexIdx);
	}
	return false;
}

void coDCEL::Check() const
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

coUint32 coDCEL::GetNbFaces() const
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

coUint32 coDCEL::GetNbNonDegenerateFaces() const
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

coUint32 coDCEL::GetNbDegenerateFaces() const
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

void coDCEL::ClearCheckedFlags() const
{
	for (const coHalfEdge& edge : halfEdges)
		edge.checked = false;
}

coUint32 coDCEL::AddFace(coUint32 faceIdx, coUint32 nbHalfEdges)
{
	const coUint32 offset = halfEdges.count;

	coReserve(halfEdges, halfEdges.count + nbHalfEdges);
	for (coUint32 idx = 0; idx < nbHalfEdges; ++idx)
	{
		const coUint32 edgeIdx = offset + idx;
		coHalfEdge e;
		e.prev = offset + (idx == 0 ? (nbHalfEdges - 1) : (idx - 1));
		e.next = offset + ((idx + 1) % nbHalfEdges);
		e.faceIdx = faceIdx;
		coPushBack(halfEdges, e);
	}
	return offset;
}

coUint32 coDCEL::AddFace(coUint32 faceIdx, const coArray<coVec3>& newVertices)
{
	coUint32 halfEdgeOffset = AddFace(faceIdx, newVertices.count);
	const coUint32 vertexOffset = vertices.count;
	coResize(vertices, vertices.count + newVertices.count);
	for (coUint32 vert = 0; vert < newVertices.count; ++vert)
	{
		const coUint32 edgeIdx = halfEdgeOffset + vert;
		const coUint32 vertIdx = vertexOffset + vert;
		coHalfEdge& e = halfEdges[edgeIdx];
		e.vertexIdx = vertIdx;
		vertices[vertIdx] = newVertices[vert];
	}
	return halfEdgeOffset;
}

coUint32 coDCEL::AddFace(coUint32 faceIdx, const coArray<coUint32>& indices)
{
	coUint32 halfEdgeOffset = AddFace(faceIdx, indices.count);
	for (coUint32 i = 0; i < indices.count; ++i)
	{
		const coUint32 edgeIdx = halfEdgeOffset + i;
		const coUint32 vertIdx = indices[i];
		coASSERT(vertIdx < vertices.count);
		coHalfEdge& e = halfEdges[edgeIdx];
		e.vertexIdx = vertIdx;
	}
	return halfEdgeOffset;
}

void coDCEL::SetTwins(coUint32 edgeAIdx, coUint32 edgeBIdx)
{
	coHalfEdge& edgeA = halfEdges[edgeAIdx];
	coHalfEdge& edgeB = halfEdges[edgeBIdx];
	coASSERT(edgeA.twin == coUint32(-1));
	coASSERT(edgeB.twin == coUint32(-1));
	edgeA.twin = edgeBIdx;
	edgeB.twin = edgeAIdx;
}

coUint32 coDCEL::GetNbAliveEdges() const
{
	coUint32 nb = 0;
	for (coUint32 i = 0; i < halfEdges.count; ++i)
	{
		if (IsEdgeAlive(i))
			++nb;
	}
	return nb;
}