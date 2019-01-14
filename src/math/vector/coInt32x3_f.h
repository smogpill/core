// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <math/vector/coInt32x3.h>

coFORCE_INLINE coInt32x3 coInt32x3_XMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_YMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0xffffffff, 0)); }
coFORCE_INLINE coInt32x3 coInt32x3_ZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0, 0)); }
coFORCE_INLINE coInt32x3 coInt32x3_XYMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0xffffffff, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_XZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_YZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0)); }
coFORCE_INLINE coInt32x3 operator& (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_and_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator| (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_or_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator^ (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_xor_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator* (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_mullo_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator+ (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_add_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator- (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_sub_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 coAbs(const coInt32x3& _this) { return coBitCast<coInt32x3>(_mm_abs_epi32(coBitCast<__m128i>(_this))); }
coFORCE_INLINE coInt32x3 coMin(const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_min_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 coMax(const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_max_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
