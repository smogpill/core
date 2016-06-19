// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/vector/Floatx3.h"
#include "math/vector/Bool32x3_f.h"
#include "math/vector/Floatx4_f.h"

namespace co
{
	coFORCE_INLINE Floatx3 make_floatx3(float _a) { return bitCast<Floatx3>(_mm_set1_ps(_a)); }
	coFORCE_INLINE Floatx3 make_floatx3(float _x, float _y, float _z) { return bitCast<Floatx3>(_mm_set_ps(_x, _y, _z, _z)); }
	coFORCE_INLINE Floatx3 make_floatx3(const Floatx3& _xxx, const Floatx3& _yyy, const Floatx3& _zzz)
	{
		const __m128 x = bitCast<__m128>(_xxx);
		const __m128 y = bitCast<__m128>(_yyy);
		const __m128 z = bitCast<__m128>(_zzz);
		return bitCast<Floatx3>(_mm_unpacklo_ps(_mm_unpacklo_ps(x, z), _mm_unpacklo_ps(y, y)));
	}
	coFORCE_INLINE float make_float(const Floatx3& _a) { return make_float(bitCast<Floatx4>(_a)); }
	coFORCE_INLINE int32 make_int32(const Floatx3& _a) { return make_int32(bitCast<Floatx4>(_a)); }
	coFORCE_INLINE uint32 make_uint32(const Floatx3& _a) { return make_uint32(bitCast<Floatx4>(_a)); }

	const Floatx3 Floatx3_ONE = make_floatx3(1.0f);
	const Floatx3 Floatx3_ZERO = make_floatx3(0.0f);

	coFORCE_INLINE Bool32x3 operator== (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) == bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 operator!= (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) != bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 operator< (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) < bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 operator<= (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) <= bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 operator> (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) > bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 operator>= (const Floatx3& _a, const Floatx3& _b) { return bitCast<Bool32x3>(bitCast<Floatx4>(_a) >= bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Floatx3 operator+ (const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(bitCast<Floatx4>(_a) + bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Floatx3 operator- (const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(bitCast<Floatx4>(_a) - bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Floatx3 operator/ (const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(bitCast<Floatx4>(_a) / bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Floatx3 operator* (const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(bitCast<Floatx4>(_a) * bitCast<Floatx4>(_b)); }
	coFORCE_INLINE Bool32x3 isValid(const Floatx3& _a) { return _a == _a; }
	template <int8 X, int8 Y, int8 Z>
	coFORCE_INLINE Floatx3 shuffle(const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(shuffle<X,Y,Z,3>(bitCast<Floatx4>(_a), bitCast<Floatx4>(_b))); }
	coFORCE_INLINE Floatx3 broadcastX(const Floatx3& _a) { return bitCast<Floatx3>(shuffle<0, 0, 0, 0>(bitCast<Floatx4>(_a), bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 broadcastY(const Floatx3& _a) { return bitCast<Floatx3>(shuffle<1, 1, 1, 1>(bitCast<Floatx4>(_a), bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 broadcastZ(const Floatx3& _a) { return bitCast<Floatx3>(shuffle<2, 2, 2, 2>(bitCast<Floatx4>(_a), bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 broadcastW(const Floatx3& _a) { return bitCast<Floatx3>(shuffle<3, 3, 3, 3>(bitCast<Floatx4>(_a), bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 abs(const Floatx3& _a) { return bitCast<Floatx3>(abs(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 sqrt(const Floatx3& _a) { return bitCast<Floatx3>(sqrt(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 inv(const Floatx3& _a) { return bitCast<Floatx3>(inv(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 invFast(const Floatx3& _a) { return bitCast<Floatx3>(invFast(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 invSqrt(const Floatx3& _a) { return bitCast<Floatx3>(invSqrt(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 invSqrtFast(const Floatx3& _a) { return bitCast<Floatx3>(invSqrtFast(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Floatx3 dot(const Floatx3& _a, const Floatx3& _b)
	{
		const Floatx3 mul = _a * _b;
		return broadcastX(mul) + broadcastY(mul) + broadcastZ(mul);
	}
	coFORCE_INLINE Floatx3 normalize(const Floatx3& _a) { return _a * invSqrt(dot(_a, _a)); }
	coFORCE_INLINE Floatx3 pow2(const Floatx3& _a) { return  _a * _a; }
	coFORCE_INLINE Floatx3 pow4(const Floatx3& _a) { return pow2(_a * _a); }
	coFORCE_INLINE Floatx3 denullify(const Floatx3& _a) { return _a + make_floatx3(+1.0e-037f); }
	coFORCE_INLINE Floatx3 min(const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(min(bitCast<Floatx4>(_a), bitCast<Floatx4>(_b))); }
	coFORCE_INLINE Floatx3 max(const Floatx3& _a, const Floatx3& _b) { return bitCast<Floatx3>(max(bitCast<Floatx4>(_a), bitCast<Floatx4>(_b))); }
	coFORCE_INLINE Floatx3 clamp(const Floatx3& _a, const Floatx3& _min, const Floatx3& _max) { return min(max(_a, _min), _max); }
	coFORCE_INLINE Floatx3 clamp01(const Floatx3& _a) { return bitCast<Floatx3>(clamp01(bitCast<Floatx4>(_a))); }
	coFORCE_INLINE Bool32x3 nearEqual(const Floatx3& _a, const Floatx3& _b, const Floatx3& _epsilon = bitCast<Floatx3>(0.0001f))
	{
		const Floatx3 delta = abs(_b - _a);
		return delta < _epsilon;
	}
	coFORCE_INLINE Floatx3 getLength(const Floatx3& _a) { return sqrt(dot(_a, _a)); }
}
