// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coHalfEdge.h"

void coHalfEdge::SwapEdgeIdx(coUint32 oldIdx, coUint32 newIdx)
{
	if (next == oldIdx)
		next = newIdx;
	if (prev == oldIdx)
		prev = newIdx;
	if (twin == oldIdx)
		twin = newIdx;
}
