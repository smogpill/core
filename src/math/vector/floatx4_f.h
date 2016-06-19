// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/floatx4.h"
#include "math/vector/bool32x4_f.h"

namespace co
{
	struct Floatx3;

	coFORCE_INLINE Floatx4 make_floatx4(float _a) { return bitCast<Floatx4>(_mm_set1_ps(_a)); }
	coFORCE_INLINE Floatx4 make_floatx4(float _x, float _y, float _z, float _w) { return bitCast<Floatx4>(_mm_set_ps(_x, _y, _z, _w)); }
	coFORCE_INLINE Floatx4 make_floatx4(const Floatx4& _xxxx, const Floatx4& _yyyy, const Floatx4& _zzzz, const Floatx4& _wwww)
	{
		const __m128 x = bitCast<__m128>(_xxxx);
		const __m128 y = bitCast<__m128>(_yyyy);
		const __m128 z = bitCast<__m128>(_zzzz);
		const __m128 w = bitCast<__m128>(_wwww);
		return bitCast<Floatx4>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, w)));
	}
	coFORCE_INLINE float make_float(const Floatx4& _a) { return _mm_cvtss_f32(bitCast<__m128>(_a)); }
	coFORCE_INLINE int32 make_int32(const Floatx4& _a) { return _mm_cvttss_si32(bitCast<__m128>(_a)); }
	coFORCE_INLINE uint32 make_uint32(const Floatx4& _a) { return static_cast<uint32>(make_int32(_a)); }

	const Floatx4 Floatx4_ONE = make_floatx4(1.0f);
	const Floatx4 Floatx4_ZERO = make_floatx4(0.0f);
	const __m128 __m128_SIGN_MASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
	const __m128 __m128_SIGN_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0));
	const __m128 __m128_MASK_XYZ = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0));
	const __m128 __m128_MASK_W = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, 0xffffffff));
	const __m128 __m128_MASK_XY = _mm_castsi128_ps(_mm_set_epi32(0xffffffff, 0xffffffff, 0, 0));
	
	coFORCE_INLINE Bool32x4 operator== (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmpeq_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 operator!= (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmpneq_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 operator< (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmplt_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 operator<= (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmple_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 operator> (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmpgt_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 operator>= (const Floatx4& _a, const Floatx4& _b) { return bitCast<Bool32x4>(_mm_cmpge_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 operator+ (const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_add_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 operator- (const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_sub_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 operator/ (const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_div_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 operator* (const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_mul_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Bool32x4 isValid(const Floatx4& _a) { return _a == _a; }
	template <int8 X, int8 Y, int8 Z, int8 W>
	coFORCE_INLINE Floatx4 shuffle(const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_shuffle_ps(bitCast<__m128>(_a), bitCast<__m128>(_b), _MM_SHUFFLE(W, Z, Y, X))); }
	coFORCE_INLINE Floatx4 broadcastX(const Floatx4& _a) { return shuffle<0, 0, 0, 0>(_a, _a); }
	coFORCE_INLINE Floatx4 broadcastY(const Floatx4& _a) { return shuffle<1, 1, 1, 1>(_a, _a); }
	coFORCE_INLINE Floatx4 broadcastZ(const Floatx4& _a) { return shuffle<2, 2, 2, 2>(_a, _a); }
	coFORCE_INLINE Floatx4 broadcastW(const Floatx4& _a) { return shuffle<3, 3, 3, 3>(_a, _a); }

	coFORCE_INLINE Floatx4 selectXYZ(const Floatx4& _a, const Floatx4& _b)
	{
		return bitCast<Floatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_XYZ, bitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_XYZ, bitCast<__m128>(_a))));
	}
	coFORCE_INLINE Floatx4 selectW(const Floatx4& _a, const Floatx4& _b)
	{
		return bitCast<Floatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_W, bitCast<__m128>(_b)), _mm_andnot_ps(__m128_MASK_W, bitCast<__m128>(_a))));
	}
	coFORCE_INLINE Floatx4 make_floatx4XYZ0(const Floatx4& _xyz)
	{
		return selectXYZ(_xyz, Floatx4_ZERO);
	}
	coFORCE_INLINE Floatx4 make_floatx4XYZ0(const Floatx3& _xyz)
	{
		return selectXYZ(bitCast<Floatx4>(_xyz), Floatx4_ZERO);
	}
	coFORCE_INLINE Floatx4 abs(const Floatx4& _a) { return bitCast<Floatx4>(_mm_andnot_ps(__m128_SIGN_MASK, bitCast<__m128>(_a))); }
	coFORCE_INLINE Floatx4 sqrt(const Floatx4& _a) { return bitCast<Floatx4>(_mm_sqrt_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE Floatx4 inv(const Floatx4& _a) { return Floatx4_ONE / _a; }
	coFORCE_INLINE Floatx4 invFast(const Floatx4& _a) { return bitCast<Floatx4>(_mm_rcp_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE Floatx4 invSqrt(const Floatx4& _a)
	{
		// newton-raphson (impl found in Vectormath)
		const __m128 a = bitCast<__m128>(_a);
		const __m128 approx = _mm_rsqrt_ps(a);
		const __m128 muls = _mm_mul_ps(_mm_mul_ps(a, approx), approx);
		const __m128 half4 = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const __m128 three = _mm_set_ps(3.0f, 3.0f, 3.0f, 3.0f);
		return bitCast<Floatx4>(_mm_mul_ps(_mm_mul_ps(half4, approx), _mm_sub_ps(three, muls)));
	}
	coFORCE_INLINE Floatx4 invSqrtFast(const Floatx4& _a) { return bitCast<Floatx4>(_mm_rsqrt_ps(bitCast<__m128>(_a))); }
	coFORCE_INLINE Floatx4 dot(const Floatx4& _a, const Floatx4& _b)
	{
		const Floatx4 mul = _a * _b;
		return broadcastX(mul) + broadcastY(mul) + broadcastZ(mul) + broadcastW(mul);
	}
	coFORCE_INLINE Floatx4 normalize(const Floatx4& _a) { return _a * invSqrt(dot(_a, _a)); }
	coFORCE_INLINE Floatx4 pow2(const Floatx4& _a) { return  _a * _a; }
	coFORCE_INLINE Floatx4 pow4(const Floatx4& _a) { return pow2(_a * _a); }
	coFORCE_INLINE Floatx4 denullify(const Floatx4& _a) { return _a + make_floatx4(+1.0e-037f); }
	coFORCE_INLINE Floatx4 min(const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_min_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 max(const Floatx4& _a, const Floatx4& _b) { return bitCast<Floatx4>(_mm_max_ps(bitCast<__m128>(_a), bitCast<__m128>(_b))); }
	coFORCE_INLINE Floatx4 clamp(const Floatx4& _a, const Floatx4& _min, const Floatx4& _max) { return min(max(_a, _min), _max); }
	coFORCE_INLINE Floatx4 clamp01(const Floatx4& _a) { return clamp(_a, Floatx4_ZERO, Floatx4_ONE); }
	coFORCE_INLINE Bool32x4 nearEqual(const Floatx4& _a, const Floatx4& _b, const Floatx4& _epsilon = bitCast<Floatx4>(0.0001f))
	{
		const Floatx4 delta = abs(_b - _a);
		return delta < _epsilon;
	}
}
