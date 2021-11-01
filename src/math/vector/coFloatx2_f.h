// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "math/vector/coFloatx2.h"
#include "math/vector/coBool32x2.h"
#include "debug/log/coAssert.h"
#include "io/stream/coBinaryOutputStream.h"
#include "io/stream/coBinaryInputStream.h"

coFORCE_INLINE coBool32x2 operator== (const coFloatx2& a, const coFloatx2& b) { return coBitCast<coBool32x2>(_mm_cmpeq_ps(coBitCast<__m128>(a), coBitCast<__m128>(b))); }
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
coFORCE_INLINE coFloatx2 coPerpendicular(const coFloatx2& _this) { return coFloatx2(-_this.y, _this.x); }
coFORCE_INLINE coBool32x2 coIsValid(const coFloatx2& _a) { return _a == _a; }
coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coFloatx2& a)
{
	stream.Write(&a, 8);
	return stream;
}
coFORCE_INLINE coBinaryInputStream& operator>>(coBinaryInputStream& stream, coFloatx2& a)
{
	stream.Read(&a, 8);
	return stream;
}
coFORCE_INLINE void coFloatx2::Write(coBinaryOutputStream& stream) const
{
	stream.Write(this, 8);
}
coFORCE_INLINE void coFloatx2::Read(coBinaryInputStream& stream)
{
	stream.Read(this, 8);
}
