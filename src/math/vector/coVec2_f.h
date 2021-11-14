// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coVec2.h"
#include "coFloatx2_f.h"

coFORCE_INLINE coFloatx4 coDot(const coVec2& _a, const coVec2& _b)
{
	const coFloatx4 mul = coBitCast<coFloatx4>(_a) * coBitCast<coFloatx4>(_b);
	return coBroadcastX(mul) + coBroadcastY(mul);
}
coFORCE_INLINE coVec2 coNormalize(const coVec2& _a) { coASSERT(!coNearEqual0(_a, 1e-10f)); return _a * coInvSquareRoot(coDot(_a, _a)); }