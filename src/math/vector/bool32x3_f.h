// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/bool32x3.h"

namespace co
{
	coFORCE_INLINE bool areAllTrue(const bool32x3& _a) { return (_mm_movemask_ps(bitCast<__m128>(_a)) & 7) == 7; }

	const bool32x3 bool32x3_false = bitCast<bool32x3>(_mm_set_ps1(0.0f));
	const bool32x3 bool32x3_true = bitCast<bool32x3>(_mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff)));

	coFORCE_INLINE bool32x3 operator|| (const bool32x3& _a, const bool32x3& _b) { return bitCast<bool32x3>(_mm_or_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x3 operator&& (const bool32x3& _a, const bool32x3& _b) { return bitCast<bool32x3>(_mm_and_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x3 operator! (const bool32x3& _a) { return bitCast<bool32x3>(_mm_xor_ps(bitCast<__m128>(bool32x3_true), bitCast<__m128>(_a))); }
}
