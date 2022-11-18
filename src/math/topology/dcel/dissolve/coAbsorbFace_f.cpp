// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAbsorbFace_f.h"
#include "../coDCEL.h"
#include <container/array/coDynamicArray_f.h>

coUint32 coAbsorbNextRadialFace(coDCEL& dcel, coUint32 edgeIdx)
{
	// Refs:
	// - bmesh_kernel_join_face_kill_edge() from https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/source/blender/bmesh/intern/bmesh_core.c

	auto& edges = dcel.halfEdges;

	const coHalfEdge& edgeA = edges[edgeIdx];
	coASSERT(!edgeA.IsDegenerate());
	coASSERT(dcel.IsEdgeManifold(edgeIdx));

	const coHalfEdge& edgeB = edges[edgeA.twin];
	const coUint32 faceA = edgeA.faceIdx;
	const coUint32 faceToAbsorb = edgeB.faceIdx;

	// Already same face?
	if (faceA == faceToAbsorb)
		return coUint32(-1);

	coASSERT(dcel.IsEdgeContiguous(edgeIdx));

	coDEBUG_CODE(dcel.CheckEdgeLoop(edgeIdx));
	coDEBUG_CODE(dcel.CheckEdgeLoop(edgeA.twin));

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
			const coUint32 bIdx = prev.twin;
			coHalfEdge& b = edges[bIdx];
			if (b.prev != a->twin)
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
			const coUint32 bIdx = next.twin;
			const coHalfEdge& b = edges[bIdx];
			if (b.next != a->twin)
				break;
			aLastRelatedToBIdx = nextIdx;
			a = &next;
		}
	}

	// Set face A on the B loop
	{
		coUint32 edgeIdxIt = edgeA.twin;
		do
		{
			coHalfEdge& edgeIt = edges[edgeIdxIt];
			edgeIt.faceIdx = faceA;
			edgeIdxIt = edgeIt.next;
		} while (edgeIdxIt != edgeA.twin);
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

	// temp
	coDynamicArray<coUint32> edgesToDisable;
	{
		coUint32 itEdgeIdx = aFirstRelatedToBIdx;
		for (;;)
		{
			coPushBack(edgesToDisable, itEdgeIdx);
			if (itEdgeIdx == aLastRelatedToBIdx)
				break;

			const coHalfEdge& e = edges[itEdgeIdx];
			coASSERT(e.next != itEdgeIdx);
			itEdgeIdx = e.next;
		}
	}


	// Link loops
	if (edges[aLastRelatedToBIdx].next == aFirstRelatedToBIdx)
	{
		const coUint32 prevIdx = edges[edges[aLastRelatedToBIdx].twin].prev;
		const coUint32 nextIdx = edges[edges[aFirstRelatedToBIdx].twin].next;
		coHalfEdge& prev = edges[prevIdx];
		coHalfEdge& next = edges[nextIdx];
		coASSERT(!prev.IsDegenerate());
		coASSERT(!next.IsDegenerate());
		prev.next = nextIdx;
		next.prev = prevIdx;
		coDEBUG_CODE(dcel.CheckEdge(prevIdx));
		coDEBUG_CODE(dcel.CheckEdge(nextIdx));
		newLoopIdx = nextIdx;
	}
	else if (edges[edges[aFirstRelatedToBIdx].twin].next == edges[aLastRelatedToBIdx].twin)
	{
		const coUint32 prevIdx = edges[aFirstRelatedToBIdx].prev;
		const coUint32 nextIdx = edges[aLastRelatedToBIdx].next;
		coHalfEdge& prev = edges[prevIdx];
		coHalfEdge& next = edges[nextIdx];
		coASSERT(!prev.IsDegenerate());
		coASSERT(!next.IsDegenerate());
		prev.next = nextIdx;
		next.prev = prevIdx;
		coDEBUG_CODE(dcel.CheckEdge(prevIdx));
		coDEBUG_CODE(dcel.CheckEdge(nextIdx));
		newLoopIdx = nextIdx;
	}
	else
	{
		{
			coHalfEdge& edge = edges[aFirstRelatedToBIdx];
			const coUint32 aIdx = edge.prev;
			newLoopIdx = aIdx;
			coHalfEdge& a = edges[aIdx];
			coASSERT(!a.IsDegenerate());
			coHalfEdge& radial = edges[edge.twin];
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
			coDEBUG_CODE(dcel.CheckEdge(aIdx));
			coDEBUG_CODE(dcel.CheckEdge(bIdx));
		}
		{
			coHalfEdge& edge = edges[aLastRelatedToBIdx];
			const coUint32 aIdx = edge.next;
			coHalfEdge& a = edges[aIdx];
			coASSERT(!a.IsDegenerate());
			coHalfEdge& radial = edges[edge.twin];
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
			coDEBUG_CODE(dcel.CheckEdge(aIdx));
			coDEBUG_CODE(dcel.CheckEdge(bIdx));
		}
	}

	coDEBUG_CODE(dcel.CheckEdgeLoop(newLoopIdx));

	// Disable any intermediate edges in the chain
	{
		if (true)
		{
			for (const coUint32 idx : edgesToDisable)
			{
				coHalfEdge& edge = edges[idx];
				const coUint32 radialIdx = edge.twin;
				coHalfEdge& radialEdge = edges[radialIdx];
				coASSERT(edge.faceIdx == faceA);
				coASSERT(radialEdge.faceIdx == faceA);

				edge.next = idx;
				edge.prev = idx;
				edge.twin = idx;
				radialEdge.next = radialIdx;
				radialEdge.prev = radialIdx;
				radialEdge.twin = radialIdx;

				coDEBUG_CODE(dcel.CheckEdge(idx));
				coDEBUG_CODE(dcel.CheckEdge(radialIdx));
			}
		}
		else
		{
			coUint32 idx = aFirstRelatedToBIdx;

			coUint32 iterations = 0;
			for (;;)
			{
				coHalfEdge& edge = edges[idx];
				const coUint32 nextIdx = edge.next;
				coASSERT(nextIdx != idx);
				const coUint32 twinIdx = edge.twin;
				coHalfEdge& twinEdge = edges[twinIdx];
				coASSERT(edge.faceIdx == faceA);
				coASSERT(twinEdge.faceIdx == faceA);

				edge.next = idx;
				edge.prev = idx;
				edge.twin = idx;
				twinEdge.next = twinIdx;
				twinEdge.prev = twinIdx;
				twinEdge.twin = twinIdx;

				coDEBUG_CODE(dcel.CheckEdge(idx));
				coDEBUG_CODE(dcel.CheckEdge(twinIdx));

				if (idx == aLastRelatedToBIdx)
					break;

				++iterations;
				idx = nextIdx;
			}
		}
	}

	coDEBUG_CODE(dcel.CheckEdgeLoop(newLoopIdx));
	//coDEBUG_CODE(dcel.Check());
	//coDEBUG_CODE(dcel.CheckNoVertexDuplicatesOnFaces());
	//coDEBUG_CODE(dcel.CheckNoMoreThan2FacesPerEdge());
	return newLoopIdx;
}