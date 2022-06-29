// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "coHalfEdge.h"
#include <math/vector/coVec3.h>

/// Warning: Not all vertices/faceNormals are used.
class coHalfEdgeMesh
{
public:
	coHalfEdgeMesh() {}
	coHalfEdgeMesh(const coArray<coUint32>& indices, const coArray<coVec3>& vertices);
	coHalfEdgeMesh(const coArray<coUint32>& indices, coUint32 nbVertices = coUint32(-1));

	void Clear();
	coUint32 AddFace(coUint32 faceIdx, coUint32 nbHalfEdges);
	void SetTwins(coUint32 edgeA, coUint32 edgeB);
	void RemoveHalfEdge(coUint32 edgeIdx);
	void CheckManifoldExceptHoles() const;
	void CheckEdgeLoop(coUint32 edgeIdx) const;
	void CheckEdge(coUint32 edgeIdx) const;
	void CheckEdgeNotReferencedByOthers(coUint32 edgeIdx) const;
	void CheckNoMoreThan2FacesPerEdge() const;
	void CheckNoVertexDuplicatesOnFaces() const;
	coBool IsEdgeAlive(coUint32 edgeIdx) const { return halfEdges[edgeIdx].next != edgeIdx; }
	coBool IsEdgeManifold(coUint32 edgeIdx) const;
	coBool IsEdgeContiguous(coUint32 edgeIdx) const;
	coBool IsEdgeBoundary(coUint32 edgeIdx) const { return halfEdges[edgeIdx].twin == edgeIdx; }
	coBool IsFaceTriangle(coUint32 edgeIdx) const;
	void Check() const;
	coUint32 GetNbFaces() const;
	coUint32 GetNbNonDegenerateFaces() const;
	coUint32 GetNbDegenerateFaces() const;
	template <class F>
	void VisitFaces(F functor) const;
	void ClearCheckedFlags() const;

	coDynamicArray<coVec3> vertices;
	coDynamicArray<coVec3> faceNormals;
	coDynamicArray<coHalfEdge> halfEdges;
};

template <class F>
void coHalfEdgeMesh::VisitFaces(F functor) const
{
	coDynamicArray<coBool> doneEdges;
	coResize(doneEdges, halfEdges.count, false);

	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count; ++edgeIdx)
	{
		if (doneEdges[edgeIdx])
			continue;

		const coHalfEdge& edge = halfEdges[edgeIdx];
		if (!functor(edge))
		{
			return;
		}

		coUint32 itIdx = edgeIdx;
		do
		{
			doneEdges[itIdx] = true;
			const coHalfEdge& itEdge = halfEdges[itIdx];
			itIdx = itEdge.next;
		} while (itIdx != edgeIdx);
	}
}

inline coBool coHalfEdgeMesh::IsFaceTriangle(coUint32 edgeIdx) const
{
	const coHalfEdge& edge = halfEdges[edgeIdx];
	const coHalfEdge& next = halfEdges[edge.next];
	return next.next == edge.prev;
}