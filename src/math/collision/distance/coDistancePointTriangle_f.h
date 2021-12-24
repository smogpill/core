// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3_f.h"

inline coFloatx4 coSquareDistancePointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c)
{
	// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
	const coVec3 ba = b - a; const coVec3 pa = p - a;
	const coVec3 cb = c - b; const coVec3 pb = p - b;
	const coVec3 ac = a - c; const coVec3 pc = p - c;
	const coVec3 nor = coCross(ba, ac);

	const coFloatx4 f = coSign(coDot(coCross(ba, nor), pa))
		+ coSign(coDot(coCross(cb, nor), pb))
		+ coSign(coDot(coCross(ac, nor), pc));
	const coFloatx4 v = f < 2.0f ?
		coMin(coMin(
			coSquareLength(ba * coClamp01(coDot(ba, pa) / coSquareLength(ba)) - pa),
			coSquareLength(cb * coClamp01(coDot(cb, pb) / coSquareLength(cb)) - pb)),
			coSquareLength(ac * coClamp01(coDot(ac, pc) / coSquareLength(ac)) - pc))
		: coDot(nor, pa) * coDot(nor, pa) / coSquareLength(nor);
	return v;
}

coVec3 coClosestPtPointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, coFloat& s, coFloat& t);