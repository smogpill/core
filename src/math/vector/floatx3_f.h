// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/floatx3.h"
#include "math/vector/bool32x3_f.h"
#include "math/vector/floatx4_f.h"

namespace co
{
	coFORCE_INLINE floatx3 make_floatx3(float _a) { return bitCast<floatx3>(_mm_set1_ps(_a)); }
	coFORCE_INLINE floatx3 make_floatx3(float _x, float _y, float _z) { return bitCast<floatx3>(_mm_set_ps(_x, _y, _z, _z)); }
	coFORCE_INLINE floatx3 make_floatx3(const floatx3& _xxx, const floatx3& _yyy, const floatx3& _zzz)
	{
		const __m128 x = bitCast<__m128>(_xxx);
		const __m128 y = bitCast<__m128>(_yyy);
		const __m128 z = bitCast<__m128>(_zzz);
		return bitCast<floatx3>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, y)));
	}
	coFORCE_INLINE float make_float(const floatx3& _a) { return make_float(bitCast<floatx4>(_a)); }
	coFORCE_INLINE int32 make_int32(const floatx3& _a) { return make_int32(bitCast<floatx4>(_a)); }
	coFORCE_INLINE uint32 make_uint32(const floatx3& _a) { return make_uint32(bitCast<floatx4>(_a)); }

	const floatx3 floatx3_ONE = make_floatx3(1.0f);
	const floatx3 floatx3_ZERO = make_floatx3(0.0f);

	coFORCE_INLINE bool32x3 operator== (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) == bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 operator!= (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) != bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 operator< (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) < bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 operator<= (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) <= bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 operator> (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) > bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 operator>= (const floatx3& _a, const floatx3& _b) { return bitCast<bool32x3>(bitCast<floatx4>(_a) >= bitCast<floatx4>(_b)); }
	coFORCE_INLINE floatx3 operator+ (const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(bitCast<floatx4>(_a) + bitCast<floatx4>(_b)); }
	coFORCE_INLINE floatx3 operator- (const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(bitCast<floatx4>(_a) - bitCast<floatx4>(_b)); }
	coFORCE_INLINE floatx3 operator/ (const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(bitCast<floatx4>(_a) / bitCast<floatx4>(_b)); }
	coFORCE_INLINE floatx3 operator* (const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(bitCast<floatx4>(_a) * bitCast<floatx4>(_b)); }
	coFORCE_INLINE bool32x3 isValid(const floatx3& _a) { return _a == _a; }
	template <int8 X, int8 Y, int8 Z>
	coFORCE_INLINE floatx3 shuffle(const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(shuffle<X,Y,Z,3>(bitCast<floatx4>(_a), bitCast<floatx4>(_b))); }
	coFORCE_INLINE floatx3 broadcastX(const floatx3& _a) { return bitCast<floatx3>(shuffle<0, 0, 0, 0>(bitCast<floatx4>(_a), bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 broadcastY(const floatx3& _a) { return bitCast<floatx3>(shuffle<1, 1, 1, 1>(bitCast<floatx4>(_a), bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 broadcastZ(const floatx3& _a) { return bitCast<floatx3>(shuffle<2, 2, 2, 2>(bitCast<floatx4>(_a), bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 broadcastW(const floatx3& _a) { return bitCast<floatx3>(shuffle<3, 3, 3, 3>(bitCast<floatx4>(_a), bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 abs(const floatx3& _a) { return bitCast<floatx3>(abs(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 sqrt(const floatx3& _a) { return bitCast<floatx3>(sqrt(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 inv(const floatx3& _a) { return bitCast<floatx3>(inv(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 invFast(const floatx3& _a) { return bitCast<floatx3>(invFast(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 invSqrt(const floatx3& _a) { return bitCast<floatx3>(invSqrt(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 invSqrtFast(const floatx3& _a) { return bitCast<floatx3>(invSqrtFast(bitCast<floatx4>(_a))); }
	coFORCE_INLINE floatx3 dot(const floatx3& _a, const floatx3& _b)
	{
		const floatx3 mul = _a * _b;
		return broadcastX(mul) + broadcastY(mul) + broadcastZ(mul);
	}
	coFORCE_INLINE floatx3 normalize(const floatx3& _a) { return _a * invSqrt(dot(_a, _a)); }
	coFORCE_INLINE floatx3 pow2(const floatx3& _a) { return  _a * _a; }
	coFORCE_INLINE floatx3 pow4(const floatx3& _a) { return pow2(_a * _a); }
	coFORCE_INLINE floatx3 denullify(const floatx3& _a) { return _a + make_floatx3(+1.0e-037f); }
	coFORCE_INLINE floatx3 min(const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(min(bitCast<floatx4>(_a), bitCast<floatx4>(_b))); }
	coFORCE_INLINE floatx3 max(const floatx3& _a, const floatx3& _b) { return bitCast<floatx3>(max(bitCast<floatx4>(_a), bitCast<floatx4>(_b))); }
	coFORCE_INLINE floatx3 clamp(const floatx3& _a, const floatx3& _min, const floatx3& _max) { return min(max(_a, _min), _max); }
	coFORCE_INLINE floatx3 clamp01(const floatx3& _a) { return bitCast<floatx3>(clamp01(bitCast<floatx4>(_a))); }
	coFORCE_INLINE bool32x3 nearEqual(const floatx3& _a, const floatx3& _b, const floatx3& _epsilon = bitCast<floatx3>(0.0001f))
	{
		const floatx3 delta = abs(_b - _a);
		return delta < _epsilon;
	}
}
