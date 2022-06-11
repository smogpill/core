// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "coHalfEdge.h"

class coHalfEdgeMesh
{
public:
	coHalfEdgeMesh() {}
	coHalfEdgeMesh(const coArray<coUint32>& indices, coUint32 nbVertices = coUint32(-1));

	coUint32 AddFace(coUint32 faceIdx, coUint32 nbHalfEdges);
	void SetRadials(coUint32 edgeA, coUint32 edgeB);
	void RemoveHalfEdge(coUint32 edgeIdx);
	void CheckEdgeLoop(coUint32 edgeIdx) const;
	void CheckEdge(coUint32 edgeIdx) const;
	coBool IsEdgeManifold(coUint32 edgeIdx) const;
	coBool IsEdgeContiguous(coUint32 edgeIdx) const;
	coBool IsEdgeBoundary(coUint32 edgeIdx) const { return halfEdges[edgeIdx].nextRadial == edgeIdx; }
	void Check() const;
	coUint32 GetNbFaces() const;
	coUint32 GetNbNonDegenerateFaces() const;
	coUint32 GetNbDegenerateFaces() const;
	template <class F>
	void VisitFaces(F functor) const;
	void ClearCheckedFlags() const;

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
