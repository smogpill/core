// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDissolveFlatVertices_f.h"
#include "../coHalfEdgeMesh.h"
#include "../../../vector/coVec3_f.h"
#include <debug/profiler/coProfile.h>

void coDissolveFlatVertices(coHalfEdgeMesh& mesh, coFloat tolerance)
{
	coPROFILE_EVENT();
	auto& edges = mesh.halfEdges;
	const auto& vertices = mesh.vertices;
	for (coUint32 a1Idx = 0; a1Idx < edges.count; ++a1Idx)
	{
		coHalfEdge& a1 = edges[a1Idx];
		const coUint32 a0Idx = a1.prev;
		const coUint32 a2Idx = a1.next;
		if (a0Idx == a2Idx || a1Idx == a2Idx)
			continue;
			
		coHalfEdge& a0 = edges[a0Idx];
		coHalfEdge& a2 = edges[a2Idx];
		const coVec3& a0Pos = vertices[a0.vertexIdx];
		const coVec3& a1Pos = vertices[a1.vertexIdx];
		const coVec3& a2Pos = vertices[a2.vertexIdx];
		const coVec3 dirA01 = coNormalize(a1Pos - a0Pos);
		const coVec3 dirA12 = coNormalize(a2Pos - a1Pos);
		if (coAbs(coDot(dirA01, dirA12) - 1.0f) > tolerance)
			continue;

		if (a0.nextRadial == a0Idx)
		{
			if (a1.nextRadial == a1Idx)
			{
				// Link 0 & 2
				a0.next = a2Idx;
				a2.prev = a0Idx;

				// Remove 1
				a1.prev = a1Idx;
				a1.next = a1Idx;
			}
		}
		else if (a1.nextRadial != a1Idx)
		{
			const coUint32 b1Idx = a0.nextRadial;
			coHalfEdge& b1 = edges[b1Idx];
			const coUint32 b0Idx = b1.prev;
			if (b0Idx != a1.nextRadial)
				continue;

			coHalfEdge& b0 = edges[b0Idx];
			const coUint32 b2Idx = b1.next;
			if (b2Idx == b1Idx)
				continue;

			coHalfEdge& b2 = edges[b2Idx];

			// Link 0 & 2
			a0.next = a2Idx;
			a0.nextRadial = b0Idx;
			a0.prevRadial = b0Idx;
			a2.prev = a0Idx;
			b0.next = b2Idx;
			b0.nextRadial = a0Idx;
			b0.prevRadial = a0Idx;
			b2.prev = b0Idx;

			// Remove 1
			a1.prev = a1Idx;
			a1.next = a1Idx;
			a1.nextRadial = a1Idx;
			a1.prevRadial = a1Idx;
			b1.prev = b1Idx;
			b1.next = b1Idx;
			b1.nextRadial = b1Idx;
			b1.prevRadial = b1Idx;
		}
	}
}
