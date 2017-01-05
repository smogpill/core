// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coBool32x4.h"

const coBool32x4 coBool32x4_false = coBitCast<coBool32x4>(_mm_set_ps1(0.0f));
const coBool32x4 coBool32x4_true = coBitCast<coBool32x4>(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)));
coFORCE_INLINE bool coIsTrueXYZ(const coBool32x4& _a) { return (_mm_movemask_ps(coBitCast<__m128>(_a)) & 7) == 7; }
coFORCE_INLINE bool coIsTrueXYZW(const coBool32x4& _a) { return (_mm_movemask_ps(coBitCast<__m128>(_a)) & 15) == 15; }
coFORCE_INLINE coBool32x4 operator==(const coBool32x4& _a, const coBool32x4& _b) { return coBitCast<coBool32x4>(_mm_cmpeq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator!=(const coBool32x4& _a, const coBool32x4& _b) { return coBitCast<coBool32x4>(_mm_cmpneq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator|| (const coBool32x4& _a, const coBool32x4& _b) { return coBitCast<coBool32x4>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator&& (const coBool32x4& _a, const coBool32x4& _b) { return coBitCast<coBool32x4>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 coNot(const coBool32x4& _a) { return coBitCast<coBool32x4>(_mm_xor_ps(coBitCast<__m128>(coBool32x4_true), coBitCast<__m128>(_a))); }
coFORCE_INLINE coBool32x4 coIsValid(const coBool32x4& _a) { return (_a == coBool32x4_false || _a == coBool32x4_true) == coBool32x4_true; }

// coFORCE_INLINE coBool32x4 operator! (const coBool32x4& _a); // not desired as it would lead to mistakes, use coNot instead (!(true, false, true, false) == (false, true, false, true)).
