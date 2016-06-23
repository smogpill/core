// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/coBool32x4.h"

coFORCE_INLINE const coBool32x4& coMake_Bool32x4(const __m128& _a) { return reinterpret_cast<const coBool32x4&>(_a); }
coFORCE_INLINE const __m128& coMake_m128(const coBool32x4& _a) { return reinterpret_cast<const __m128&>(_a); }
coFORCE_INLINE bool coIsTrueXYZ(const coBool32x4& _a) { return (_mm_movemask_ps(coMake_m128(_a)) & 7) == 7; }
coFORCE_INLINE bool coIsTrueXYZW(const coBool32x4& _a) { return (_mm_movemask_ps(coMake_m128(_a)) & 15) == 15; }

const coBool32x4 coBool32x4_false = coMake_Bool32x4(_mm_set_ps1(0.0f));
const coBool32x4 coBool32x4_true = coMake_Bool32x4(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)));

coFORCE_INLINE coBool32x4 operator|| (const coBool32x4& _a, const coBool32x4& _b) { return coMake_Bool32x4(_mm_or_ps(coMake_m128(_a), coMake_m128(_b))); }
coFORCE_INLINE coBool32x4 operator&& (const coBool32x4& _a, const coBool32x4& _b) { return coMake_Bool32x4(_mm_and_ps(coMake_m128(_a), coMake_m128(_b))); }
coFORCE_INLINE coBool32x4 operator! (const coBool32x4& _a) { return coMake_Bool32x4(_mm_xor_ps(coMake_m128(coBool32x4_true), coMake_m128(_a))); }
