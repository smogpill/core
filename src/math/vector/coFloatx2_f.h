// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coFloatx2.h"
#include "debug/log/coAssert.h"

coFORCE_INLINE coFloatx2 operator+ (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_add_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2& operator+= (coFloatx2& _this, const coFloatx2& _b) { return _this = coBitCast<coFloatx2>(_mm_add_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator- (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_sub_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator- (const coFloatx2& _a) { return coBitCast<coFloatx2>(_mm_sub_ps(_mm_setzero_ps(), coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx2& operator-= (coFloatx2& _this, const coFloatx2& _b) { return _this = coBitCast<coFloatx2>(_mm_sub_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator/ (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_div_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2& operator/= (coFloatx2& _this, const coFloatx2& _b) { return _this = coBitCast<coFloatx2>(_mm_div_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator* (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_mul_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2& operator*= (coFloatx2& _this, const coFloatx2& _b) { return _this = coBitCast<coFloatx2>(_mm_mul_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator& (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx2 operator| (const coFloatx2& _a, const coFloatx2& _b) { return coBitCast<coFloatx2>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
