// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coTriangle.h"
#include "math/vector/coVec3_f.h"

coFORCE_INLINE coVec3 coGetRawNormal(const coTriangle& t)
{
	return coCross(t.b - t.a, t.c - t.a);
}

coFORCE_INLINE coVec3 coGetNormal(const coTriangle& t)
{
	return coNormalize(coGetRawNormal(t));
}

coFORCE_INLINE coBool32x4 coOverlapInfiniteExtrude(const coTriangle& t, const coVec3& p)
{
	// https://gdbooks.gitbooks.io/3dcollisions/content/Chapter4/point_in_triangle.html
	const coVec3 pa = t.a - p;
	const coVec3 pb = t.b - p;
	const coVec3 pc = t.c - p;
	const coVec3 u = coCross(pb, pc);
	const coVec3 v = coCross(pc, pa);
	const coVec3 w = coCross(pa, pb);
	return coDot(u, v) >= 0.0f && coDot(u, w) >= 0.0f;
}
