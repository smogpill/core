// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coCollapseEdges_f.h"
#include "../coHalfEdgeMesh.h"
#include "../visit/coVisit_f.h"
#include "../../../vector/coVec3_f.h"

void coDissolveDegenerateEdge(coHalfEdgeMesh& mesh, coUint32 edgeIdx)
{
	const coUint32 aIdx = edgeIdx;
	coHalfEdge& a = mesh.halfEdges[aIdx];
	coASSERT(a.IsDegenerate());
	const coUint32 bIdx = a.next;
	coHalfEdge& b = mesh.halfEdges[bIdx];
	const coUint32 aTwinIdx = a.twin;
	const coUint32 bTwinIdx = b.twin;
	coHalfEdge& aTwin = mesh.halfEdges[aTwinIdx];
	coHalfEdge& bTwin = mesh.halfEdges[bTwinIdx];
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

void coCollapseEdge(coHalfEdgeMesh& mesh, coUint32 edgeIdx)
{
	coASSERT(coIsCollapsible(mesh, edgeIdx));
	coHalfEdge& edge = mesh.halfEdges[edgeIdx];
	if (edge.next == edgeIdx)
		return;
	const coUint32 twinIdx = edge.twin;
	coHalfEdge& twin = mesh.halfEdges[twinIdx];
	const coUint32 prevIdx = edge.prev;
	const coUint32 nextIdx = edge.next;
	const coUint32 prevTwinIdx = twin.prev;
	const coUint32 nextTwinIdx = twin.next;
	coHalfEdge& prev = mesh.halfEdges[prevIdx];
	coHalfEdge& next = mesh.halfEdges[nextIdx];
	coHalfEdge& prevTwin = mesh.halfEdges[prevTwinIdx];
	coHalfEdge& nextTwin = mesh.halfEdges[nextTwinIdx];

	// Collapse vertices
	{
		coVec3& edgeVert = mesh.vertices[edge.vertexIdx];
		const coVec3& nextVert = mesh.vertices[next.vertexIdx];
		edgeVert = (edgeVert + nextVert) * 0.5f;
	}

	{
		const coUint32 aIdx = edge.vertexIdx;
		const coUint32 bIdx = next.vertexIdx;
		auto changeVertex = [&](coUint32 e)
		{
			coASSERT(mesh.halfEdges[e].vertexIdx == bIdx);
			mesh.halfEdges[e].vertexIdx = aIdx;
			return true;
		};
		coVisitHalfEdgeFanAroundVertex(mesh, edge.next, changeVertex);
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

	coDEBUG_CODE(mesh.CheckEdge(prevIdx));
	coDEBUG_CODE(mesh.CheckEdge(nextIdx));
	coDEBUG_CODE(mesh.CheckEdge(prevTwinIdx));
	coDEBUG_CODE(mesh.CheckEdge(nextTwinIdx));

	// Todo: dissolve degenerate faces
	if (next.IsDegenerate())
	{
		coDissolveDegenerateEdge(mesh, nextIdx);
	}
	if (nextTwin.IsDegenerate())
	{
		coDissolveDegenerateEdge(mesh, nextTwinIdx);
	}

	//mesh.CheckNoVertexDuplicatesOnFaces();
}

coBool coCollapseEdgeIfSmallerThanSquaredDist(coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, coFloat squaredDist)
{
	const coHalfEdge& edge = mesh.halfEdges[halfEdgeIdx];
	const coHalfEdge& next = mesh.halfEdges[edge.next];
	const coUint32 aIdx = edge.vertexIdx;
	const coUint32 bIdx = next.vertexIdx;
	coVec3& a = mesh.vertices[edge.vertexIdx];
	const coVec3& b = mesh.vertices[next.vertexIdx];
	if (coSquareLength(b - a) < squaredDist)
	{
		a = (a + b) * 0.5f;
		coCollapseEdge(mesh, halfEdgeIdx);
		mesh.CheckNoMoreThan2FacesPerEdge();
		return true;
	}
	return false;
}

void coCollapseEdgesSmallerThanDist(coHalfEdgeMesh& mesh, coFloat distance)
{
	mesh.CheckNoMoreThan2FacesPerEdge();
	const coFloat squaredDist = distance * distance;
	coDynamicArray<coUint32> scratch;
	for (coUint32 edgeIdx = 0; edgeIdx < mesh.halfEdges.count; ++edgeIdx)
	{
		if (coIsCollapsible(mesh, edgeIdx, scratch))
			coCollapseEdgeIfSmallerThanSquaredDist(mesh, edgeIdx, squaredDist);
	}
	mesh.CheckNoMoreThan2FacesPerEdge();
}

coBool coIsCollapsible(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx)
{
	coDynamicArray<coUint32> scratch;
	return coIsCollapsible(mesh, halfEdgeIdx, scratch);
}

coBool coIsCollapsible(const coHalfEdgeMesh& mesh, coUint32 halfEdgeIdx, coDynamicArray<coUint32>& scratch)
{
	const auto& edges = mesh.halfEdges;
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
	coVisitHalfEdgeFanAroundVertex(mesh, halfEdgeIdx, gatherNextVertices);

	auto compareWithOtherVertices = [&](const coUint32 circleEdgeIdx)
	{
		const coHalfEdge& circleEdge = edges[circleEdgeIdx];
		const coHalfEdge& nextCircleEdge = edges[circleEdge.next];
		const coUint32 circleVertex = nextCircleEdge.vertexIdx;
		if (coContains(scratch, circleVertex))
			return false;
		return true;
	};
	return coVisitHalfEdgeFanAroundVertex(mesh, edge.next, compareWithOtherVertices);
}
