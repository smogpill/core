// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coInt32x4.h"

coFORCE_INLINE coInt32x4 operator& (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coInt32x4 operator| (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coInt32x4 operator^ (const coInt32x4& _a, const coInt32x4& _b) { return coBitCast<coInt32x4>(_mm_xor_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
