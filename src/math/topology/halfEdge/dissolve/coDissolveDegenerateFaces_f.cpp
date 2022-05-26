// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include <container/array/coArray_f.h>
#include "../coHalfEdgeMesh.h"
#include "coDissolveDegenerateFaces_f.h"

void coDissolveDegenerateFaces(coHalfEdgeMesh& mesh)
{
	auto& halfEdges = mesh.halfEdges;
	for (coUint32 edgeIdx = 0; edgeIdx < halfEdges.count;)
	{
		coHalfEdge& edge = halfEdges[edgeIdx];
		if (edge.next == edge.prev)
		{
			mesh.RemoveHalfEdge(edgeIdx);
			continue;
		}
		++edgeIdx;
	}
}