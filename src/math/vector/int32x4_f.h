// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/int32x4.h"

namespace co
{
	coFORCE_INLINE int32x4 operator& (const int32x4& _a, const int32x4& _b) { return bitCast<int32x4>(_mm_and_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE int32x4 operator| (const int32x4& _a, const int32x4& _b) { return bitCast<int32x4>(_mm_or_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE int32x4 operator^ (const int32x4& _a, const int32x4& _b) { return bitCast<int32x4>(_mm_xor_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
}
