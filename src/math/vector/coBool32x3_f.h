// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coBool32x3.h"

coFORCE_INLINE bool coAreAllTrue(const coBool32x3& _a) { return (_mm_movemask_ps(coBitCast<__m128>(_a)) & 7) == 7; }

const coBool32x3 coBool32x3_false = coBitCast<coBool32x3>(_mm_set_ps1(0.0f));
const coBool32x3 coBool32x3_true = coBitCast<coBool32x3>(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)));

coFORCE_INLINE coBool32x3 operator|| (const coBool32x3& _a, const coBool32x3& _b) { return coBitCast<coBool32x3>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 operator&& (const coBool32x3& _a, const coBool32x3& _b) { return coBitCast<coBool32x3>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 operator! (const coBool32x3& _a) { return coBitCast<coBool32x3>(_mm_xor_ps(coBitCast<__m128>(coBool32x3_true), coBitCast<__m128>(_a))); }
