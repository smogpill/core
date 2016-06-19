// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/bool32x4.h"

namespace co
{
	coFORCE_INLINE const Bool32x4& make_bool32x4(const __m128& _a) { return reinterpret_cast<const Bool32x4&>(_a); }
	coFORCE_INLINE const __m128& make_m128(const Bool32x4& _a) { return reinterpret_cast<const __m128&>(_a); }
	coFORCE_INLINE bool isTrueXYZ(const Bool32x4& _a) { return (_mm_movemask_ps(make_m128(_a)) & 7) == 7; }
	coFORCE_INLINE bool isTrueXYZW(const Bool32x4& _a) { return (_mm_movemask_ps(make_m128(_a)) & 15) == 15; }

	const Bool32x4 Bool32x4_false = make_bool32x4(_mm_set_ps1(0.0f));
	const Bool32x4 Bool32x4_true = make_bool32x4(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)));

	coFORCE_INLINE Bool32x4 operator|| (const Bool32x4& _a, const Bool32x4& _b) { return make_bool32x4(_mm_or_ps(make_m128(_a), make_m128(_b))); }
	coFORCE_INLINE Bool32x4 operator&& (const Bool32x4& _a, const Bool32x4& _b) { return make_bool32x4(_mm_and_ps(make_m128(_a), make_m128(_b))); }
	coFORCE_INLINE Bool32x4 operator! (const Bool32x4& _a) { return make_bool32x4(_mm_xor_ps(make_m128(Bool32x4_true), make_m128(_a))); }
}
