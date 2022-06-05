// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "../coHalfEdgeMesh.h"
#include "coAbsorbFace_f.h"

coBool coAbsorbNextRadialFace(coHalfEdgeMesh& mesh, coUint32 edgeIdx)
{
	auto& edges = mesh.halfEdges;

	const coHalfEdge& edgeA = edges[edgeIdx];
	coASSERT(!edgeA.IsDegenerate());

	const coHalfEdge& edgeB = edges[edgeA.nextRadial];
	const coUint32 faceA = edgeA.faceIdx;
	const coUint32 faceToAbsorb = edgeB.faceIdx;

	// Already same face?
	if (faceA == faceToAbsorb)
		return false;

	coDEBUG_CODE(mesh.CheckEdgeLoop(edgeIdx));
	coDEBUG_CODE(mesh.CheckEdgeLoop(edgeA.nextRadial));

	// Find first in the chain
	coUint32 aFirstRelatedToBIdx = edgeIdx;
	{
		const coHalfEdge* a = &edges[aFirstRelatedToBIdx];
		for (;;)
		{
			const coUint32 prevIdx = a->prev;
			if (prevIdx == edgeIdx)
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
	coUint32 aLastRelatedToBIdx = edgeIdx;
	{
		const coHalfEdge* a = &edges[aLastRelatedToBIdx];
		for (;;)
		{
			const coUint32 nextIdx = a->next;
			if (nextIdx == edgeIdx)
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
		coUint32 edgeIdxIt = edgeA.nextRadial;
		do
		{
			coHalfEdge& edgeIt = edges[edgeIdxIt];
			edgeIt.faceIdx = faceA;
			edgeIdxIt = edgeIt.next;
		} while (edgeIdxIt != edgeA.nextRadial);
	}

	coUint32 newLoopIdx;

	coUint32 debug_0_aIdx;
	coUint32 debug_0_bIdx;
	coUint32 debug_0_a_next;
	coUint32 debug_0_b_prev;

	coUint32 debug_1_aIdx;
	coUint32 debug_1_bIdx;
	coUint32 debug_1_a_prev;
	coUint32 debug_1_b_next;

	// Link loops
	{
		coHalfEdge& edge = edges[aFirstRelatedToBIdx];
		const coUint32 aIdx = edge.prev;
		newLoopIdx = aIdx;
		coHalfEdge& a = edges[aIdx];
		coASSERT(!a.IsDegenerate());
		coHalfEdge& radial = edges[edge.nextRadial];
		coASSERT(!radial.IsDegenerate());
		const coUint32 bIdx = radial.next;
		coHalfEdge& b = edges[bIdx];
		coASSERT(!b.IsDegenerate());
		coASSERT(aIdx != bIdx);
		debug_0_aIdx = aIdx;
		debug_0_bIdx = bIdx;
		debug_0_a_next = a.next;
		debug_0_b_prev = b.prev;
		coASSERT(a.next != bIdx);
		coASSERT(b.prev != aIdx);
		a.next = bIdx;
		b.prev = aIdx;
		coDEBUG_CODE(mesh.CheckEdge(aIdx));
		coDEBUG_CODE(mesh.CheckEdge(bIdx));
	}
	{
		coHalfEdge& edge = edges[aLastRelatedToBIdx];
		const coUint32 aIdx = edge.next;
		coHalfEdge& a = edges[aIdx];
		coASSERT(!a.IsDegenerate());
		coHalfEdge& radial = edges[edge.nextRadial];
		coASSERT(!radial.IsDegenerate());
		const coUint32 bIdx = radial.prev;
		coHalfEdge& b = edges[bIdx];
		coASSERT(!b.IsDegenerate());
		coASSERT(aIdx != bIdx);
		debug_1_aIdx = aIdx;
		debug_1_bIdx = bIdx;
		debug_1_a_prev = a.prev;
		debug_1_b_next = b.next;
		coASSERT(a.prev != bIdx);
		coASSERT(b.next != aIdx);
		a.prev = bIdx;
		b.next = aIdx;
		coDEBUG_CODE(mesh.CheckEdge(aIdx));
		coDEBUG_CODE(mesh.CheckEdge(bIdx));
	}

	coDEBUG_CODE(mesh.CheckEdgeLoop(newLoopIdx));

	// Disable any intermediate edges in the chain
	{
		coUint32 idx = aFirstRelatedToBIdx;
		for (;;)
		{
			coHalfEdge& edge = edges[idx];
			const coUint32 nextIdx = edge.next;
			coHalfEdge& radialEdge = edges[edge.nextRadial];
			const coUint32 radialIdx = edge.nextRadial;
			coASSERT(edge.faceIdx == faceA);
			coASSERT(radialEdge.faceIdx == faceA);

			edge.next = idx;
			edge.prev = idx;
			edge.nextRadial = idx;
			edge.prevRadial = idx;
			radialEdge.next = radialIdx;
			radialEdge.prev = radialIdx;
			radialEdge.nextRadial = radialIdx;
			radialEdge.prevRadial = radialIdx;

			coDEBUG_CODE(mesh.CheckEdge(idx));
			coDEBUG_CODE(mesh.CheckEdge(radialIdx));

			if (idx == aLastRelatedToBIdx)
				break;
			idx = nextIdx;
		}
	}

	coDEBUG_CODE(mesh.CheckEdgeLoop(newLoopIdx));
	coDEBUG_CODE(mesh.Check());
	return true;
}