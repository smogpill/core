// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coBool32x3_f.h"
#include "math/vector/coFloatx4_f.h"
#include "debug/log/coAssert.h"
#include "io/stream/coBinaryOutputStream.h"
#include "io/stream/coBinaryInputStream.h"

coFORCE_INLINE coFloatx3 coMake_Floatx3(coFloat _a) { return coBitCast<coFloatx3>(_mm_set1_ps(_a)); }
coFORCE_INLINE coFloatx3 coMake_Floatx3(coFloat _x, coFloat _y, coFloat _z) { return coBitCast<coFloatx3>(_mm_set_ps(_z, _z, _y, _x)); }
coFORCE_INLINE coFloatx3 coMake_Floatx3(const coFloatx3& _xxx, const coFloatx3& _yyy, const coFloatx3& _zzz)
{
	const __m128 x = coBitCast<__m128>(_xxx);
	const __m128 y = coBitCast<__m128>(_yyy);
	const __m128 z = coBitCast<__m128>(_zzz);
	return coBitCast<coFloatx3>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, y)));
}
coFORCE_INLINE float coMake_Float(const coFloatx3& _a) { return coMake_Float(coBitCast<coFloatx4>(_a)); }
coFORCE_INLINE coInt32 coMake_Int32(const coFloatx3& _a) { return coMake_Int32(coBitCast<coFloatx4>(_a)); }
coFORCE_INLINE coUint32 coMake_Uint32(const coFloatx3& _a) { return coMake_Uint32(coBitCast<coFloatx4>(_a)); }

const coFloatx3 coFloatx3_ONE = coMake_Floatx3(1.0f);
const coFloatx3 coFloatx3_ZERO = coMake_Floatx3(0.0f);
const coFloatx3 coFloatx3_DEGREES_TO_RADIANS = coMake_Floatx3(0.01745329252f);
const coFloatx3 coFloatx3_RADIANS_TO_DEGREES = coMake_Floatx3(57.2957795131f);

coFORCE_INLINE coFloatx3 coFloatx3_XMask() { return coBitCast<coFloatx3>(_mm_set_epi32(0, 0, 0, 0xffffffff)); }
coFORCE_INLINE coFloatx3 coFloatx3_YMask() { return coBitCast<coFloatx3>(_mm_set_epi32(0, 0, 0xffffffff, 0)); }
coFORCE_INLINE coFloatx3 coFloatx3_ZMask() { return coBitCast<coFloatx3>(_mm_set_epi32(0, 0xffffffff, 0, 0)); }

coFORCE_INLINE coBool32x3 operator== (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmpeq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
//coFORCE_INLINE coBool32x3 operator!= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmpneq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); } Removed because cast to coBool is not intuitive.
coFORCE_INLINE coBool32x3 operator< (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmplt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 operator<= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmple_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 operator> (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmpgt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 operator>= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(_mm_cmpge_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator+ (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_add_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3& operator+= (coFloatx3& _this, const coFloatx3& _b) { return _this = coBitCast<coFloatx3>(_mm_add_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator- (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_sub_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator- (const coFloatx3& _a) { return coBitCast<coFloatx3>(_mm_sub_ps(_mm_setzero_ps(), coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx3 operator+ (const coFloatx3& _a) { return _a; }
coFORCE_INLINE coFloatx3& operator-= (coFloatx3& _this, const coFloatx3& _b) { return _this = coBitCast<coFloatx3>(_mm_sub_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator/ (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_div_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3& operator/= (coFloatx3& _this, const coFloatx3& _b) { return _this = coBitCast<coFloatx3>(_mm_div_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator* (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_mul_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3& operator*= (coFloatx3& _this, const coFloatx3& _b) { return _this = coBitCast<coFloatx3>(_mm_mul_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator& (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx3 operator| (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x3 coIsValid(const coFloatx3& _a) { return _a == _a; }
template <coInt8 X, coInt8 Y, coInt8 Z>
coFORCE_INLINE coFloatx3 coShuffle(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coShuffle<X, Y, Z, 3>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
template <coInt8 X, coInt8 Y, coInt8 Z>
coFORCE_INLINE coFloatx3 coShuffle(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<X, Y, Z, 3>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastX(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<0, 0, 0, 0>(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastY(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<1, 1, 1, 1>(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastZ(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<2, 2, 2, 2>(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coAbs(const coFloatx3& _a) { return coBitCast<coFloatx3>(coAbs(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coSquareRoot(const coFloatx3& _a) { return coBitCast<coFloatx3>(coSquareRoot(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInv(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInv(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvFast(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvFast(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvSquareRoot(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvSquareRoot(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvSqrtFast(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvSqrtFast(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coPow2(const coFloatx3& _a) { return  _a * _a; }
coFORCE_INLINE coFloatx3 coPow4(const coFloatx3& _a) { return coPow2(_a * _a); }
coFORCE_INLINE coFloatx3 coDenullify(const coFloatx3& _a) { return _a + coMake_Floatx3(+1.0e-037f); }
coFORCE_INLINE coFloatx3 coMin(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coMin(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
coFORCE_INLINE coFloatx3 coMin(const coFloatx3& _a) { return coMin(coBroadcastX(_a), coMin(coBroadcastY(_a), coBroadcastZ(_a))); }
coFORCE_INLINE coFloatx3 coMax(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coMax(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
coFORCE_INLINE coFloatx3 coMax(const coFloatx3& _a) { return coMax(coBroadcastX(_a), coMax(coBroadcastY(_a), coBroadcastZ(_a))); }
coFORCE_INLINE coFloatx3 coMaxWith0(const coFloatx3& _a) { return coBitCast<coFloatx3>(coMaxWith0(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coClamp(const coFloatx3& _a, const coFloatx3& _min, const coFloatx3& _max) { return coMin(coMax(_a, _min), _max); }
coFORCE_INLINE coFloatx3 coClamp01(const coFloatx3& _a) { return coBitCast<coFloatx3>(coClamp01(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coBool32x3 coNearEqual(const coFloatx3& _a, const coFloatx3& _b, const coFloatx3& _epsilon = coFloatx3(coFloat_NearEqualDefaultEpsilon)) { return coAbs(_b - _a) < _epsilon; }
coFORCE_INLINE coBool32x3 coNearEqual0(const coFloatx3& _a, const coFloatx3& _epsilon = coFloatx3(coFloat_NearEqualDefaultEpsilon)) { return coAbs(_a) < _epsilon; }
coFORCE_INLINE coBool32x3 coAreXYZEqual(const coFloatx3& _a) { const coFloatx3 x = coBroadcastX(_a); return x == coBroadcastY(_a) && x == coBroadcastZ(_a); }
coFORCE_INLINE coFloatx3 coSin(const coFloatx3& _a) { return coBitCast<coFloatx3>(coSin(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coCos(const coFloatx3& _a) { return coBitCast<coFloatx3>(coCos(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coMod(coFloatx3 _a, coFloatx3 _b) { return coFloatx3(coMod(_a.x, _b.x), coMod(_a.y, _b.y), coMod(_a.z, _b.z)); }
coFORCE_INLINE coFloatx3 coConvertToRad(coFloatx3 degrees) { return degrees * 0.01745329252f; }
coFORCE_INLINE coFloatx3 coConvertToDeg(coFloatx3 radians) { return radians * 57.2957795131f; }
coFORCE_INLINE coFloatx3 coRand01x3(coUint32& seed) { return coFloatx3(coRand01(seed), coRand01(seed), coRand01(seed)); }
coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coFloatx3& a)
{
	stream.Write(&a, 12);
	return stream;
}
coFORCE_INLINE coBinaryInputStream& operator>>(coBinaryInputStream& stream, coFloatx3& a)
{
	stream.Read(&a, 12);
	return stream;
}
coFORCE_INLINE void coFloatx3::Write(coBinaryOutputStream& stream) const
{
	stream.Write(this, 12);
}
coFORCE_INLINE void coFloatx3::Read(coBinaryInputStream& stream)
{
	stream.Read(this, 12);
}
