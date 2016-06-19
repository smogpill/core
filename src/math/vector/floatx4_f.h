// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/floatx4.h"
#include "math/vector/bool32x4_f.h"

namespace co
{
	coFORCE_INLINE floatx4 make_floatx4(float _a) { return bitCast<floatx4>(_mm_set1_ps(_a)); }
	coFORCE_INLINE floatx4 make_floatx4(float _x, float _y, float _z, float _w) { return bitCast<floatx4>(_mm_set_ps(_x, _y, _z, _w)); }
	coFORCE_INLINE floatx4 make_floatx4(const floatx4& _xxxx, const floatx4& _yyyy, const floatx4& _zzzz, const floatx4& _wwww)
	{
		const __m128 x = bitCast<__m128>(_xxxx);
		const __m128 y = bitCast<__m128>(_yyyy);
		const __m128 z = bitCast<__m128>(_zzzz);
		const __m128 w = bitCast<__m128>(_wwww);
		return bitCast<floatx4>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, w)));
	}
	coFORCE_INLINE float make_float(const floatx4& _a) { return _mm_cvtss_f32(bitCast<__m128>(_a)); }
	coFORCE_INLINE int32 make_int32(const floatx4& _a) { return _mm_cvttss_si32(bitCast<__m128>(_a)); }
	coFORCE_INLINE uint32 make_uint32(const floatx4& _a) { return static_cast<uint32>(make_int32(_a)); }

	const floatx4 floatx4_ONE = make_floatx4(1.0f);
	const floatx4 floatx4_ZERO = make_floatx4(0.0f);
	const __m128 __m128_SIGN_MASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
	const __m128 __m128_SIGN_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0));
	const __m128 __m128_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0));
	const __m128 __m128_MASK_W = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0xffffffff));
	const __m128 __m128_MASK_XY = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0, 0));
	
	coFORCE_INLINE bool32x4 operator== (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmpeq_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 operator!= (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmpneq_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 operator< (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmplt_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 operator<= (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmple_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 operator> (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmpgt_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 operator>= (const floatx4& _a, const floatx4& _b) { return bitCast<bool32x4>(_mm_cmpge_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 operator+ (const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_add_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 operator- (const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_sub_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 operator/ (const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_div_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 operator* (const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_mul_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE bool32x4 isValid(const floatx4& _a) { return _a == _a; }
	template <int8 X, int8 Y, int8 Z, int8 W>
	coFORCE_INLINE floatx4 shuffle(const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_shuffle_ps(bitCast<__m128>(_a), bitCast<__m128>(_b), _MM_SHUFFLE(W, Z, Y, X))); }
	coFORCE_INLINE floatx4 broadcastX(const floatx4& _a) { return shuffle<0, 0, 0, 0>(_a, _a); }
	coFORCE_INLINE floatx4 broadcastY(const floatx4& _a) { return shuffle<1, 1, 1, 1>(_a, _a); }
	coFORCE_INLINE floatx4 broadcastZ(const floatx4& _a) { return shuffle<2, 2, 2, 2>(_a, _a); }
	coFORCE_INLINE floatx4 broadcastW(const floatx4& _a) { return shuffle<3, 3, 3, 3>(_a, _a); }

	coFORCE_INLINE floatx4 selectXYZ(const floatx4& _a, const floatx4& _b)
	{
		return bitCast<floatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_XYZ, bitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_XYZ, bitCast<__m128>(_a))));
	}
	coFORCE_INLINE floatx4 selectW(const floatx4& _a, const floatx4& _b)
	{
		return bitCast<floatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_W, bitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_W, bitCast<__m128>(_a))));
	}
	coFORCE_INLINE floatx4 abs(const floatx4& _a) { return bitCast<floatx4>(_mm_andnot_ps(__m128_SIGN_MASK, bitCast<__m128>(_a))); }
	coFORCE_INLINE floatx4 sqrt(const floatx4& _a) { return bitCast<floatx4>(_mm_sqrt_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE floatx4 inv(const floatx4& _a) { return floatx4_ONE / _a; }
	coFORCE_INLINE floatx4 invFast(const floatx4& _a) { return bitCast<floatx4>(_mm_rcp_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE floatx4 invSqrt(const floatx4& _a)
	{
		// newton-raphson (impl found in Vectormath)
		const __m128 a = bitCast<__m128>(_a);
		const __m128 approx = _mm_rsqrt_ps(a);
		const __m128 muls = _mm_mul_ps(_mm_mul_ps(a, approx), approx);
		const __m128 half4 = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const __m128 three = _mm_set_ps(3.0f, 3.0f, 3.0f, 3.0f);
		return bitCast<floatx4>(_mm_mul_ps(_mm_mul_ps(half4, approx), _mm_sub_ps(three, muls)));
	}
	coFORCE_INLINE floatx4 invSqrtFast(const floatx4& _a) { return bitCast<floatx4>(_mm_rsqrt_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE floatx4 dot(const floatx4& _a, const floatx4& _b)
	{
		const floatx4 mul = _a * _b;
		return broadcastX(mul) + broadcastY(mul) + broadcastZ(mul) + broadcastW(mul);
	}
	coFORCE_INLINE floatx4 normalize(const floatx4& _a) { return _a * invSqrt(dot(_a, _a)); }
	coFORCE_INLINE floatx4 pow2(const floatx4& _a) { return  _a * _a; }
	coFORCE_INLINE floatx4 pow4(const floatx4& _a) { return pow2(_a * _a); }
	coFORCE_INLINE floatx4 denullify(const floatx4& _a) { return _a + make_floatx4(+1.0e-037f); }
	coFORCE_INLINE floatx4 min(const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_min_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 max(const floatx4& _a, const floatx4& _b) { return bitCast<floatx4>(_mm_max_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE floatx4 clamp(const floatx4& _a, const floatx4& _min, const floatx4& _max) { return min(max(_a, _min), _max); }
	coFORCE_INLINE floatx4 clamp01(const floatx4& _a) { return clamp(_a, floatx4_ZERO, floatx4_ONE); }
	coFORCE_INLINE bool32x4 nearEqual(const floatx4& _a, const floatx4& _b, const floatx4& _epsilon = bitCast<floatx4>(0.0001f))
	{
		const floatx4 delta = abs(_b - _a);
		return delta < _epsilon;
	}
}
