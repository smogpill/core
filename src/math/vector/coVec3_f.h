// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3.h"
#include "math/vector/coBool32x3_f.h"
#include "math/vector/coFloatx3_f.h"
#include "debug/log/coAssert.h"

coFORCE_INLINE coFloatx4 coDot(const coVec3& _a, const coVec3& _b)
{
	const coFloatx4 mul = coBitCast<coFloatx4>(_a) * coBitCast<coFloatx4>(_b);
	return coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul);
}
coFORCE_INLINE coVec3 coCross(const coVec3& _a, const coVec3& _b)
{
	const coVec3 tmp0 = coShuffle<1, 2, 0>(_a, _a);
	const coVec3 tmp1 = coShuffle<2, 0, 1>(_b, _b);
	const coVec3 tmp2 = coShuffle<2, 0, 1>(_a, _a);
	const coVec3 tmp3 = coShuffle<1, 2, 0>(_b, _b);
	return tmp0 * tmp1 - tmp2 * tmp3;
}
coFORCE_INLINE coFloatx4 coSquareLength(const coVec3& _a) { return coDot(_a, _a); }
coFORCE_INLINE coFloatx4 coLength(const coVec3& _a) { return coSquareRoot(coDot(_a, _a)); }
coFORCE_INLINE coVec3 coNormalize(const coVec3& _a) { coASSERT(!coNearEqual0(_a)); return _a*coInvSquareRoot(coDot(_a, _a)); }
coFORCE_INLINE coBool32x3 coIsNormalized(const coVec3& _a, const coVec3& _squareEpsilon = coVec3(1e-3f)) { return coNearEqual(coSquareLength(_a), coVec3(1.0f), _squareEpsilon); }
coFORCE_INLINE coVec3 coAnyOrthogonal(const coVec3& _a)
{
	const coFloatx3 absA = coAbs(_a);
	switch (coMinIndex(absA.x, absA.y, absA.z))
	{
	case 0: return coCross(_a, coVec3(1.0f, 0.0f, 0.0f));
	case 1: return coCross(_a, coVec3(0.0f, 1.0f, 0.0f));
	case 2: return coCross(_a, coVec3(0.0f, 0.0f, 1.0f));
	default: coASSERT(false); return coVec3();
	}
}
