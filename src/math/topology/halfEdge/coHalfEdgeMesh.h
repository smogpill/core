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

	void RemoveHalfEdge(coUint32 edgeIdx);
	coBool IsEdgeLoopValid(coUint32 edgeIdx) const;

	coUint32 GetNbFaces() const;
	coUint32 GetNbNonDegenerateFaces() const;
	coUint32 GetNbDegenerateFaces() const;
	template <class F>
	void VisitFaces(F functor) const;

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
