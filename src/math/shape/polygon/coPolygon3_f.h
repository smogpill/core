// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPolygon3.h"
#include <math/vector/coBool32x3_f.h>
#include <math/vector/coVec3_f.h>

class TriangulateScratch
{
public:
	coDynamicArray<coUint32> remainingIndices;
	coDynamicArray<coUint32> sorted;
	coDynamicArray<coFloat> deviations;
};

void coTriangulateXY(const coPolygon3& poly, coDynamicArray<coUint32>& triangles, TriangulateScratch& scratch);
coFloat coSignedAreaXY(const coPolygon3& poly);
coFORCE_INLINE coBool coIsClockwiseXY(const coPolygon3& poly) { return coSignedAreaXY(poly) < 0.0f; }
coFORCE_INLINE coBool coIsCornerConvexXY(const coVec3& v0, const coVec3& v1, const coVec3& v2)
{
	return !coAreAllFalse(coBroadcastZ(coCross(v0 - v1, v2 - v1)) >= coFloatx3(0.0f));
}
coFORCE_INLINE coBool coIsInsideTriangleXY(const coVec3& v0, const coVec3& v1, const coVec3& v2, const coVec3& vP)
{
	const coBool b1 = coIsCornerConvexXY(vP, v0, v1);
	const coBool b2 = coIsCornerConvexXY(vP, v1, v2);
	const coBool b3 = coIsCornerConvexXY(vP, v2, v0);

	return (b1 == b2) & (b2 == b3);
}
