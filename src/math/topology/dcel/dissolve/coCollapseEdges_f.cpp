// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coCollapseEdges_f.h"
#include "../coDCEL.h"
#include "../visit/coVisit_f.h"
#include "../../../vector/coVec3_f.h"

void coDissolveDegenerateEdge(coDCEL& dcel, coUint32 edgeIdx)
{
	const coUint32 aIdx = edgeIdx;
	coHalfEdge& a = dcel.halfEdges[aIdx];
	coASSERT(a.IsDegenerate());
	const coUint32 bIdx = a.next;
	coHalfEdge& b = dcel.halfEdges[bIdx];
	const coUint32 aTwinIdx = a.twin;
	const coUint32 bTwinIdx = b.twin;
	coHalfEdge& aTwin = dcel.halfEdges[aTwinIdx];
	coHalfEdge& bTwin = dcel.halfEdges[bTwinIdx];
	coASSERT(aTwin.twin == aIdx);
	coASSERT(bTwin.twin == bIdx);
	aTwin.twin = bTwinIdx == bIdx ? aTwinIdx : bTwinIdx;
	bTwin.twin = aTwinIdx == aIdx ? bTwinIdx : aTwinIdx;
	a.next = aIdx;
	a.prev = aIdx;
	a.twin = aIdx;
	b.next = bIdx;
	b.prev = bIdx;
	b.twin = bIdx;
}

void coCollapseEdge(coDCEL& dcel, coUint32 edgeIdx)
{
	coASSERT(coIsCollapsible(dcel, edgeIdx));
	coHalfEdge& edge = dcel.halfEdges[edgeIdx];
	if (edge.next == edgeIdx)
		return;
	const coUint32 twinIdx = edge.twin;
	coHalfEdge& twin = dcel.halfEdges[twinIdx];
	const coUint32 prevIdx = edge.prev;
	const coUint32 nextIdx = edge.next;
	const coUint32 prevTwinIdx = twin.prev;
	const coUint32 nextTwinIdx = twin.next;
	coHalfEdge& prev = dcel.halfEdges[prevIdx];
	coHalfEdge& next = dcel.halfEdges[nextIdx];
	coHalfEdge& prevTwin = dcel.halfEdges[prevTwinIdx];
	coHalfEdge& nextTwin = dcel.halfEdges[nextTwinIdx];

	// Collapse vertices
	{
		coVec3& edgeVert = dcel.vertices[edge.vertexIdx];
		const coVec3& nextVert = dcel.vertices[next.vertexIdx];
		edgeVert = (edgeVert + nextVert) * 0.5f;
	}

	{
		const coUint32 aIdx = edge.vertexIdx;
		const coUint32 bIdx = next.vertexIdx;
		auto changeVertex = [&](coUint32 e)
		{
			coASSERT(dcel.halfEdges[e].vertexIdx == bIdx);
			dcel.halfEdges[e].vertexIdx = aIdx;
			return true;
		};
		coVisitHalfEdgeFanAroundVertex(dcel, edge.next, changeVertex);
	}

	// Link
	prev.next = nextIdx;
	next.prev = prevIdx;
	prevTwin.next = nextTwinIdx;
	nextTwin.prev = prevTwinIdx;

	edge.prev = edgeIdx;
	edge.next = edgeIdx;
	edge.twin = edgeIdx;

	twin.prev = twinIdx;
	twin.next = twinIdx;
	twin.twin = twinIdx;

	coDEBUG_CODE(dcel.CheckEdge(prevIdx));
	coDEBUG_CODE(dcel.CheckEdge(nextIdx));
	coDEBUG_CODE(dcel.CheckEdge(prevTwinIdx));
	coDEBUG_CODE(dcel.CheckEdge(nextTwinIdx));

	// Todo: dissolve degenerate faces
	if (next.IsDegenerate())
	{
		coDissolveDegenerateEdge(dcel, nextIdx);
	}
	if (nextTwin.IsDegenerate())
	{
		coDissolveDegenerateEdge(dcel, nextTwinIdx);
	}

	//dcel.CheckNoVertexDuplicatesOnFaces();
}

coBool coCollapseEdgeIfSmallerThanSquaredDist(coDCEL& dcel, coUint32 halfEdgeIdx, coFloat squaredDist)
{
	const coHalfEdge& edge = dcel.halfEdges[halfEdgeIdx];
	const coHalfEdge& next = dcel.halfEdges[edge.next];
	const coUint32 aIdx = edge.vertexIdx;
	const coUint32 bIdx = next.vertexIdx;
	coVec3& a = dcel.vertices[edge.vertexIdx];
	const coVec3& b = dcel.vertices[next.vertexIdx];
	if (coSquareLength(b - a) < squaredDist)
	{
		a = (a + b) * 0.5f;
		coCollapseEdge(dcel, halfEdgeIdx);
		dcel.CheckNoMoreThan2FacesPerEdge();
		return true;
	}
	return false;
}

void coCollapseEdgesSmallerThanDist(coDCEL& dcel, coFloat distance)
{
	dcel.CheckNoMoreThan2FacesPerEdge();
	const coFloat squaredDist = distance * distance;
	coDynamicArray<coUint32> scratch;
	for (coUint32 edgeIdx = 0; edgeIdx < dcel.halfEdges.count; ++edgeIdx)
	{
		if (coIsCollapsible(dcel, edgeIdx, scratch))
			coCollapseEdgeIfSmallerThanSquaredDist(dcel, edgeIdx, squaredDist);
	}
	dcel.CheckNoMoreThan2FacesPerEdge();
}

coBool coIsCollapsible(const coDCEL& dcel, coUint32 halfEdgeIdx)
{
	coDynamicArray<coUint32> scratch;
	return coIsCollapsible(dcel, halfEdgeIdx, scratch);
}

coBool coIsCollapsible(const coDCEL& dcel, coUint32 halfEdgeIdx, coDynamicArray<coUint32>& scratch)
{
	const auto& edges = dcel.halfEdges;
	const coHalfEdge& edge = edges[halfEdgeIdx];
	if (edge.next == halfEdgeIdx)
		return false;

	coClear(scratch);
	auto gatherNextVertices = [&](const coUint32 circleEdgeIdx)
	{
		const coHalfEdge& circleEdge = edges[circleEdgeIdx];
		const coHalfEdge& nextCircleEdge = edges[circleEdge.next];
		const coUint32 circleVertex = nextCircleEdge.vertexIdx;
		coPushBack(scratch, circleVertex);
		return true;
	};
	coVisitHalfEdgeFanAroundVertex(dcel, halfEdgeIdx, gatherNextVertices);

	auto compareWithOtherVertices = [&](const coUint32 circleEdgeIdx)
	{
		const coHalfEdge& circleEdge = edges[circleEdgeIdx];
		const coHalfEdge& nextCircleEdge = edges[circleEdge.next];
		const coUint32 circleVertex = nextCircleEdge.vertexIdx;
		if (coContains(scratch, circleVertex))
			return false;
		return true;
	};
	return coVisitHalfEdgeFanAroundVertex(dcel, edge.next, compareWithOtherVertices);
}
