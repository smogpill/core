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
	const coUint32 aRadialIdx = a.nextRadial;
	const coUint32 bRadialIdx = b.nextRadial;
	coHalfEdge& aRadial = mesh.halfEdges[aRadialIdx];
	coHalfEdge& bRadial = mesh.halfEdges[bRadialIdx];
	coASSERT(aRadial.nextRadial == aIdx);
	coASSERT(bRadial.nextRadial == bIdx);
	aRadial.nextRadial = bRadialIdx;
	aRadial.prevRadial = bRadialIdx;
	bRadial.nextRadial = aRadialIdx;
	bRadial.prevRadial = aRadialIdx;
	a.next = aIdx;
	a.prev = aIdx;
	a.nextRadial = aIdx;
	a.prevRadial = aIdx;
	b.next = bIdx;
	b.prev = bIdx;
	b.nextRadial = bIdx;
	b.prevRadial = bIdx;
}

void coCollapseEdge(coHalfEdgeMesh& mesh, coUint32 edgeIdx)
{
	coHalfEdge& edge = mesh.halfEdges[edgeIdx];
	if (edge.next == edgeIdx)
		return;
	const coUint32 radialIdx = edge.nextRadial;
	coHalfEdge& radial = mesh.halfEdges[radialIdx];
	const coUint32 prevIdx = edge.prev;
	const coUint32 nextIdx = edge.next;
	const coUint32 prevRadialIdx = radial.prev;
	const coUint32 nextRadialIdx = radial.next;
	coHalfEdge& prev = mesh.halfEdges[prevIdx];
	coHalfEdge& next = mesh.halfEdges[nextIdx];
	coHalfEdge& prevRadial = mesh.halfEdges[prevRadialIdx];
	coHalfEdge& nextRadial = mesh.halfEdges[nextRadialIdx];

	{
		const coUint32 aIdx = edge.vertexIdx;
		const coUint32 bIdx = next.vertexIdx;
		auto changeVertex = [&](coUint32 e)
		{
			coASSERT(mesh.halfEdges[e].vertexIdx == bIdx);
			mesh.halfEdges[e].vertexIdx = aIdx;
		};
		coVisitAllHalfEdgesAroundVertex(mesh, edge.next, changeVertex);
	}

	// Link
	prev.next = nextIdx;
	next.prev = prevIdx;
	prevRadial.next = nextRadialIdx;
	nextRadial.prev = prevRadialIdx;

	edge.prev = edgeIdx;
	edge.next = edgeIdx;
	edge.prevRadial = edgeIdx;
	edge.nextRadial = edgeIdx;

	radial.prev = radialIdx;
	radial.next = radialIdx;
	radial.prevRadial = radialIdx;
	radial.nextRadial = radialIdx;

	coDEBUG_CODE(mesh.CheckEdge(prevIdx));
	coDEBUG_CODE(mesh.CheckEdge(nextIdx));
	coDEBUG_CODE(mesh.CheckEdge(prevRadialIdx));
	coDEBUG_CODE(mesh.CheckEdge(nextRadialIdx));

	// Todo: dissolve degenerate faces
	if (next.IsDegenerate())
	{
		coDissolveDegenerateEdge(mesh, nextIdx);
	}
	if (nextRadial.IsDegenerate())
	{
		coDissolveDegenerateEdge(mesh, nextRadialIdx);
	}

	mesh.CheckNoVertexDuplicatesOnFaces();
}

void coCollapseEdge(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coUint32 edgeIdx)
{
	const coHalfEdge& edge = mesh.halfEdges[edgeIdx];
	const coHalfEdge& next = mesh.halfEdges[edge.next];
	const coUint32 aIdx = edge.vertexIdx;
	const coUint32 bIdx = next.vertexIdx;
	coVec3& a = vertices[edge.vertexIdx];
	const coVec3& b = vertices[next.vertexIdx];
	a = (a + b) * 0.5f;
	coCollapseEdge(mesh, edgeIdx);
}

coBool coCollapseEdgeIfSmallerThanSquaredDist(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coUint32 halfEdgeIdx, coFloat squaredDist)
{
	const coHalfEdge& edge = mesh.halfEdges[halfEdgeIdx];
	const coHalfEdge& next = mesh.halfEdges[edge.next];
	const coUint32 aIdx = edge.vertexIdx;
	const coUint32 bIdx = next.vertexIdx;
	coVec3& a = vertices[edge.vertexIdx];
	const coVec3& b = vertices[next.vertexIdx];
	if (coSquareLength(b - a) < squaredDist)
	{
		a = (a + b) * 0.5f;
		coCollapseEdge(mesh, halfEdgeIdx);
		return true;
	}
	return false;
}

void coCollapseEdgesSmallerThanDist(coHalfEdgeMesh& mesh, coArray<coVec3>& vertices, coFloat distance)
{
	mesh.CheckNoMoreThan2FacesPerEdge();
	const coFloat squaredDist = distance * distance;
	for (coUint32 edgeIdx = 0; edgeIdx < mesh.halfEdges.count; ++edgeIdx)
	{
		coCollapseEdgeIfSmallerThanSquaredDist(mesh, vertices, edgeIdx, squaredDist);
		mesh.CheckNoMoreThan2FacesPerEdge();
	}
	mesh.CheckNoMoreThan2FacesPerEdge();
}
