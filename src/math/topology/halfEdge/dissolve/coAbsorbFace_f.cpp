// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "../coHalfEdgeMesh.h"
#include "coAbsorbFace_f.h"

coBool coAbsorbFace(coHalfEdgeMesh& mesh, coUint32 anyEdgeIdx, coUint32 faceToAbsorb)
{
	auto& edges = mesh.halfEdges;
	coUint32 aAnyRelatedToBIdx = coUint32(-1);
	coUint32 bAnyIdx = coUint32(-1);
	{
		coUint32 aIdx = anyEdgeIdx;
		do
		{
			coHalfEdge& a = edges[aIdx];
			const coUint32 bIdx = a.nextRadial;
			if (bIdx != aIdx)
			{
				coHalfEdge& b = edges[bIdx];
				if (b.faceIdx == faceToAbsorb)
				{
					aAnyRelatedToBIdx = aIdx;
					bAnyIdx = bIdx;
					break;
				}
			}
			aIdx = a.next;
		} while (aIdx != anyEdgeIdx);
	}

	if (aAnyRelatedToBIdx == coUint32(-1))
		return false;

	// Find first in the chain
	coUint32 aFirstRelatedToBIdx = aAnyRelatedToBIdx;
	{
		const coHalfEdge* a = &edges[aFirstRelatedToBIdx];
		for (;;)
		{
			const coUint32 prevIdx = a->prev;
			if (prevIdx == aAnyRelatedToBIdx)
				break;

			const coHalfEdge& prev = edges[prevIdx];
			const coUint32 bIdx = prev.nextRadial;
			if (bIdx == prevIdx)
				break;

			coHalfEdge& b = edges[bIdx];
			if (b.faceIdx != faceToAbsorb)
				break;
			aFirstRelatedToBIdx = prevIdx;
			a = &prev;
		}
	}

	// Find last in the chain
	coUint32 aLastRelatedToBIdx = aAnyRelatedToBIdx;
	{
		const coHalfEdge* a = &edges[aLastRelatedToBIdx];
		for (;;)
		{
			const coUint32 nextIdx = a->next;
			if (nextIdx == aAnyRelatedToBIdx)
				break;

			const coHalfEdge& next = edges[nextIdx];
			const coUint32 bIdx = next.nextRadial;
			if (bIdx == nextIdx)
				break;

			coHalfEdge& b = edges[bIdx];
			if (b.faceIdx != faceToAbsorb)
				break;
			aLastRelatedToBIdx = nextIdx;
			a = &next;
		}
	}

	// Set face A on the B loop
	{
		const coUint32 faceA = edges[anyEdgeIdx].faceIdx;
		coUint32 edgeIdxIt = bAnyIdx;
		do
		{
			coHalfEdge& edgeIt = edges[edgeIdxIt];
			edgeIt.faceIdx = faceA;
			edgeIdxIt = edgeIt.next;
		} while (edgeIdxIt != bAnyIdx);
	}

	// Link loops
	{
		coHalfEdge& edge = edges[aFirstRelatedToBIdx];
		const coUint32 aIdx = edge.prev;
		coHalfEdge& a = edges[aIdx];
		coHalfEdge& radial = edges[edge.nextRadial];
		const coUint32 bIdx = radial.next;
		coHalfEdge& b = edges[bIdx];
		a.next = bIdx;
		b.prev = aIdx;
	}
	if (aLastRelatedToBIdx != aFirstRelatedToBIdx)
	{
		coHalfEdge& edge = edges[aLastRelatedToBIdx];
		const coUint32 aIdx = edge.prev;
		coHalfEdge& a = edges[aIdx];
		coHalfEdge& radial = edges[edge.nextRadial];
		const coUint32 bIdx = radial.next;
		coHalfEdge& b = edges[bIdx];
		a.next = bIdx;
		b.prev = aIdx;
	}

	// Disable any intermediate edges in the chain
	{
		coUint32 idx = aFirstRelatedToBIdx;
		for (;;)
		{
			coHalfEdge& edge = edges[idx];
			const coUint32 nextIdx = edge.next;
			edge.next = idx;
			edge.prev = idx;
			if (idx == aLastRelatedToBIdx)
				break;
			idx = nextIdx;
		}
	}

	return true;
}