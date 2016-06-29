// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coFloatx4.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coBool32x4_f.h"

class coFloatx3;

coFORCE_INLINE coFloatx4 coMake_floatx4(coFloat _a) { return coBitCast<coFloatx4>(_mm_set1_ps(_a)); }
coFORCE_INLINE coFloatx4 coMake_floatx4(coFloat _x, coFloat _y, coFloat _z, coFloat _w) { return coBitCast<coFloatx4>(_mm_set_ps(_x, _y, _z, _w)); }
coFORCE_INLINE coFloatx4 coMake_floatx4(const coFloatx4& _xxxx, const coFloatx4& _yyyy, const coFloatx4& _zzzz, const coFloatx4& _wwww)
{
	const __m128 x = coBitCast<__m128>(_xxxx);
	const __m128 y = coBitCast<__m128>(_yyyy);
	const __m128 z = coBitCast<__m128>(_zzzz);
	const __m128 w = coBitCast<__m128>(_wwww);
	return coBitCast<coFloatx4>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, w)));
}
coFORCE_INLINE coFloat coMake_Float(const coFloatx4& _a) { return _mm_cvtss_f32(coBitCast<__m128>(_a)); }
coFORCE_INLINE coInt32 coMake_Int32(const coFloatx4& _a) { return _mm_cvttss_si32(coBitCast<__m128>(_a)); }
coFORCE_INLINE coUint32 coMake_Uint32(const coFloatx4& _a) { return static_cast<coUint32>(coMake_Int32(_a)); }

const coFloatx4 coFloatx4_ONE = coMake_floatx4(1.0f);
const coFloatx4 coFloatx4_ZERO = coMake_floatx4(0.0f);
const __m128 __m128_SIGN_MASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
const __m128 __m128_SIGN_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0));
const __m128 __m128_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0));
const __m128 __m128_MASK_W = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0xffffffff));
const __m128 __m128_MASK_XY = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0, 0));
	
coFORCE_INLINE coBool32x4 operator== (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpeq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator!= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpneq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator< (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmplt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator<= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmple_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator> (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpgt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator>= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpge_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator+ (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_add_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator- (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_sub_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator/ (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_div_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator* (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_mul_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 coIsValid(const coFloatx4& _a) { return _a == _a; }
template <coInt8 X, coInt8 Y, coInt8 Z, coInt8 W>
coFORCE_INLINE coFloatx4 coShuffle(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_shuffle_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b), _MM_SHUFFLE(W, Z, Y, X))); }
coFORCE_INLINE coFloatx4 coBroadcastX(const coFloatx4& _a) { return coShuffle<0, 0, 0, 0>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastY(const coFloatx4& _a) { return coShuffle<1, 1, 1, 1>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastZ(const coFloatx4& _a) { return coShuffle<2, 2, 2, 2>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastW(const coFloatx4& _a) { return coShuffle<3, 3, 3, 3>(_a, _a); }

coFORCE_INLINE coFloatx4 coSelectXYZ(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_XYZ, coBitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_XYZ, coBitCast<__m128>(_a))));
}
coFORCE_INLINE coFloatx4 coSelectW(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_W, coBitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_W, coBitCast<__m128>(_a))));
}
coFORCE_INLINE coFloatx4 coMake_floatx4XYZ0(const coFloatx4& _xyz)
{
	return coSelectXYZ(_xyz, coFloatx4_ZERO);
}
coFORCE_INLINE coFloatx4 coMake_floatx4XYZ0(const coFloatx3& _xyz)
{
	return coSelectXYZ(coBitCast<coFloatx4>(_xyz), coFloatx4_ZERO);
}
coFORCE_INLINE coFloatx4 coAbs(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_andnot_ps(__m128_SIGN_MASK, coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coSqrt(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_sqrt_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coInv(const coFloatx4& _a) { return coFloatx4_ONE / _a; }
coFORCE_INLINE coFloatx4 coInvFast(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_rcp_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coInvSqrt(const coFloatx4& _a)
{
	// newton-raphson (impl found in Vectormath)
	const __m128 a = coBitCast<__m128>(_a);
	const __m128 approx = _mm_rsqrt_ps(a);
	const __m128 muls = _mm_mul_ps(_mm_mul_ps(a, approx), approx);
	const __m128 half4 = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
	const __m128 three = _mm_set_ps(3.0f, 3.0f, 3.0f, 3.0f);
	return coBitCast<coFloatx4>(_mm_mul_ps(_mm_mul_ps(half4, approx), _mm_sub_ps(three, muls)));
}
coFORCE_INLINE coFloatx4 coInvSqrtFast(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_rsqrt_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coDot(const coFloatx4& _a, const coFloatx4& _b)
{
	const coFloatx4 mul = _a * _b;
	return coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul) + coBroadcastW(mul);
}
coFORCE_INLINE coFloatx4 coNormalize(const coFloatx4& _a) { return _a * coInvSqrt(coDot(_a, _a)); }
coFORCE_INLINE coFloatx4 coPow2(const coFloatx4& _a) { return  _a * _a; }
coFORCE_INLINE coFloatx4 coPow4(const coFloatx4& _a) { return coPow2(_a * _a); }
coFORCE_INLINE coFloatx4 coDenullify(const coFloatx4& _a) { return _a + coMake_floatx4(+1.0e-037f); }
coFORCE_INLINE coFloatx4 coMin(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_min_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 coMax(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_max_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 coClamp(const coFloatx4& _a, const coFloatx4& _min, const coFloatx4& _max) { return coMin(coMax(_a, _min), _max); }
coFORCE_INLINE coFloatx4 coClamp01(const coFloatx4& _a) { return coClamp(_a, coFloatx4_ZERO, coFloatx4_ONE); }
coFORCE_INLINE coBool32x4 coNearEqual(const coFloatx4& _a, const coFloatx4& _b, const coFloatx4& _epsilon = coBitCast<coFloatx4>(0.0001f))
{
	const coFloatx4 delta = coAbs(_b - _a);
	return delta < _epsilon;
}
