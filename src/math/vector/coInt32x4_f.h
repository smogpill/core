// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "math/vector/coInt32x4.h"

const __m128 __m128_SIGN_MASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
const __m128 __m128_SIGN_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0, 0x80000000, 0x80000000, 0x80000000));
const __m128 __m128_SIGN_MASK_W = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0, 0, 0));
const __m128 __m128_MASK_X = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0xffffffff));
const __m128 __m128_MASK_Y = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0xffffffff, 0));
const __m128 __m128_MASK_Z = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0, 0));
const __m128 __m128_MASK_W = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0, 0, 0));
const __m128 __m128_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff));
const __m128 __m128_MASK_XY = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0xffffffff, 0xffffffff));

const coInt32x4 coInt32x4_SIGN_MASK = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set1_epi32(0x80000000)));
const coInt32x4 coInt32x4_SIGN_MASK_XYZ = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set_epi32(0, 0x80000000, 0x80000000, 0x80000000)));
const coInt32x4 coInt32x4_SIGN_MASK_W = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set_epi32(0x80000000, 0, 0, 0)));
const coInt32x4 coInt32x4_MASK_XYZ = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff)));
const coInt32x4 coInt32x4_MASK_W = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0, 0, 0)));
const coInt32x4 coInt32x4_MASK_XY = coBitCast<coInt32x4>(_mm_castsi128_ps(_mm_set_epi32(0, 0, 0xffffffff, 0xffffffff)));

coFORCE_INLINE coInt32x4 operator& (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_and_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x4 operator| (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_or_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x4 operator^ (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_xor_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
