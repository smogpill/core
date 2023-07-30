// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec4.h"
#include "math/vector/coFloatx4_f.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coBool32x4_f.h"
#include "math/vector/coInt32x4_f.h"

coFORCE_INLINE coFloatx4 coDot(const coVec4& _a, const coVec4& _b)
{
	const coVec4 mul = _a * _b;
	return coFloatx4(coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul) + coBroadcastW(mul));
}
coFORCE_INLINE coVec4 coNormalize(const coVec4& _a, const coVec4& epsilon = coVec4(1e-10f)) { return coNearEqual0(_a, epsilon) ? coVec4(0.0f) : _a * coInvSquareRoot(coDot(_a, _a)); }
coFORCE_INLINE coFloatx4 coSquareLength(const coVec4& _a) { return coDot(_a, _a); }
coFORCE_INLINE coFloatx4 coLength(const coVec4& a) { return coSquareRoot(coDot(a, a)); }
coFORCE_INLINE coBool32x4 coIsNormalized(const coVec4& _a, const coVec4& _squareEpsilon = coVec4(1e-3f)) { return coNearEqual(coSquareLength(_a), coVec4(1.0f), _squareEpsilon); }
