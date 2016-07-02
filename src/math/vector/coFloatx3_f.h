// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coBool32x3_f.h"
#include "math/vector/coFloatx4_f.h"

coFORCE_INLINE coFloatx3 coMake_Floatx3(coFloat _a) { return coBitCast<coFloatx3>(_mm_set1_ps(_a)); }
coFORCE_INLINE coFloatx3 coMake_Floatx3(coFloat _x, coFloat _y, coFloat _z) { return coBitCast<coFloatx3>(_mm_set_ps(_x, _y, _z, _z)); }
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

coFORCE_INLINE coBool32x3 operator== (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) == coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 operator!= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) != coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 operator< (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) < coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 operator<= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) <= coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 operator> (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) > coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 operator>= (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coBool32x3>(coBitCast<coFloatx4>(_a) >= coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coFloatx3 operator+ (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coBitCast<coFloatx4>(_a) + coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coFloatx3 operator- (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coBitCast<coFloatx4>(_a) - coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coFloatx3 operator/ (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coBitCast<coFloatx4>(_a) / coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coFloatx3 operator* (const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coBitCast<coFloatx4>(_a) * coBitCast<coFloatx4>(_b)); }
coFORCE_INLINE coBool32x3 coIsValid(const coFloatx3& _a) { return _a == _a; }
template <coInt8 X, coInt8 Y, coInt8 Z>
coFORCE_INLINE coFloatx3 coShuffle(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coShuffle<X, Y, Z, 3>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
coFORCE_INLINE coFloatx3 coBroadcastX(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<0, 0, 0, 0>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastY(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<1, 1, 1, 1>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastZ(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<2, 2, 2, 2>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coBroadcastW(const coFloatx3& _a) { return coBitCast<coFloatx3>(coShuffle<3, 3, 3, 3>(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coAbs(const coFloatx3& _a) { return coBitCast<coFloatx3>(coAbs(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coSqrt(const coFloatx3& _a) { return coBitCast<coFloatx3>(coSqrt(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInv(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInv(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvFast(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvFast(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvSqrt(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvSqrt(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coInvSqrtFast(const coFloatx3& _a) { return coBitCast<coFloatx3>(coInvSqrtFast(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coFloatx3 coDot(const coFloatx3& _a, const coFloatx3& _b)
{
	const coFloatx3 mul = _a * _b;
	return coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul);
}
coFORCE_INLINE coFloatx3 coNormalize(const coFloatx3& _a) { return _a * coInvSqrt(coDot(_a, _a)); }
coFORCE_INLINE coFloatx3 coPow2(const coFloatx3& _a) { return  _a * _a; }
coFORCE_INLINE coFloatx3 coPow4(const coFloatx3& _a) { return coPow2(_a * _a); }
coFORCE_INLINE coFloatx3 coDenullify(const coFloatx3& _a) { return _a + coMake_Floatx3(+1.0e-037f); }
coFORCE_INLINE coFloatx3 coMin(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coMin(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
coFORCE_INLINE coFloatx3 coMax(const coFloatx3& _a, const coFloatx3& _b) { return coBitCast<coFloatx3>(coMax(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b))); }
coFORCE_INLINE coFloatx3 coClamp(const coFloatx3& _a, const coFloatx3& _min, const coFloatx3& _max) { return coMin(coMax(_a, _min), _max); }
coFORCE_INLINE coFloatx3 coClamp01(const coFloatx3& _a) { return coBitCast<coFloatx3>(coClamp01(coBitCast<coFloatx4>(_a))); }
coFORCE_INLINE coBool32x3 coNearEqual(const coFloatx3& _a, const coFloatx3& _b, const coFloatx3& _epsilon = coBitCast<coFloatx3>(0.0001f))
{
	const coFloatx3 delta = coAbs(_b - _a);
	return delta < _epsilon;
}
coFORCE_INLINE coFloatx3 getLength(const coFloatx3& _a) { return coSqrt(coDot(_a, _a)); }