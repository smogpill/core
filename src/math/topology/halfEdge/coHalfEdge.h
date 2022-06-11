// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coHalfEdge
{
public:
	coHalfEdge() : checked(false) {}
	void SwapEdgeIdx(coUint32 oldIdx, coUint32 newIdx);
	coBool IsDegenerate() const { return prev == next; }

	coUint32 next = coUint32(-1);
	coUint32 prev = coUint32(-1);
	coUint32 nextRadial = coUint32(-1);
	coUint32 prevRadial = coUint32(-1);
	coUint32 vertexIdx = coUint32(-1);
	coUint32 faceIdx = coUint32(-1);
	mutable coBool checked : 1;
};
