// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUint32x3.h"

coFORCE_INLINE coUint32x3 operator- (const coUint32x3& a, const coUint32x3& b) { return coBitCast<coUint32x3>(_mm_sub_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }