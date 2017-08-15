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

coFORCE_INLINE coVec4 coDot(const coVec4& _a, const coVec4& _b)
{
	const coVec4 mul = _a * _b;
	return coVec4(coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul) + coBroadcastW(mul));
}
coFORCE_INLINE coVec4 coNormalize(const coVec4& _a) { return _a * coInvSquareRoot(coDot(_a, _a)); }
