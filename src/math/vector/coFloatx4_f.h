// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coFloatx4.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coBool32x4_f.h"
#include "math/vector/coInt32x4_f.h"
#include "io/stream/coBinaryOutputStream.h"
#include "io/stream/coBinaryInputStream.h"

class coFloatx3;

namespace coFloatx4_
{
	const coFloatx4 inv2Pi = coFloat_inv2Pi;
	const coFloatx4 halfPi = coFloat_halfPi;
};

coFORCE_INLINE coFloatx4 coMake_floatx4(coFloat _a) { return coBitCast<coFloatx4>(_mm_set1_ps(_a)); }
coFORCE_INLINE coFloatx4 coMake_floatx4(coFloat _x, coFloat _y, coFloat _z, coFloat _w) { return coBitCast<coFloatx4>(_mm_set_ps(_w, _z, _y, _x)); }
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
	
coFORCE_INLINE coBool32x4 operator== (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpeq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
// coFORCE_INLINE coBool32x4 operator!= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpneq_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); } // Removed because cast to coBool is not intuitive.
coFORCE_INLINE coBool32x4 operator< (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmplt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator<= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmple_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator> (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpgt_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coBool32x4 operator>= (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coBool32x4>(_mm_cmpge_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator+ (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_add_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator- (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_sub_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator/ (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_div_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator* (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_mul_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator& (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_and_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator| (const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_or_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 operator- (const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_sub_ps(_mm_setzero_ps(), coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4& operator+= (coFloatx4& _this, const coFloatx4& _b) { return _this = coBitCast<coFloatx4>(_mm_add_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4& operator-= (coFloatx4& _this, const coFloatx4& _b) { return _this = coBitCast<coFloatx4>(_mm_sub_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4& operator*= (coFloatx4& _this, const coFloatx4& _b) { return _this = coBitCast<coFloatx4>(_mm_mul_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4& operator/= (coFloatx4& _this, const coFloatx4& _b) { return _this = coBitCast<coFloatx4>(_mm_div_ps(coBitCast<__m128>(_this), coBitCast<__m128>(_b))); }
//coFORCE_INLINE coFloatx4 coMulAdd(const coFloatx4& a, const coFloatx4& b, const coFloatx4& c) { return coBitCast<coFloatx4>(_mm_fmadd_ps(coBitCast<__m128>(a), coBitCast<__m128>(b), coBitCast<__m128>(c))); }
coFORCE_INLINE coFloatx4 coMulAdd(const coFloatx4& a, const coFloatx4& b, const coFloatx4& c) { return a * b + c; }
coFORCE_INLINE coFloatx4 coAddSub(const coFloatx4& a, const coFloatx4& b) { return coBitCast<coFloatx4>(_mm_addsub_ps(coBitCast<__m128>(a), coBitCast<__m128>(b))); }
coFORCE_INLINE coBool32x4 coIsValid(const coFloatx4& _a) { return _a == _a; }
template <coInt8 X, coInt8 Y, coInt8 Z, coInt8 W>
coFORCE_INLINE coFloatx4 coShuffle(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_shuffle_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b), _MM_SHUFFLE(W, Z, Y, X))); }
template <coInt8 X, coInt8 Y, coInt8 Z, coInt8 W>
coFORCE_INLINE coFloatx4 coShuffle(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_shuffle_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_a), _MM_SHUFFLE(W, Z, Y, X))); }
coFORCE_INLINE coFloatx4 coBroadcastX(const coFloatx4& _a) { return coShuffle<0, 0, 0, 0>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastY(const coFloatx4& _a) { return coShuffle<1, 1, 1, 1>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastZ(const coFloatx4& _a) { return coShuffle<2, 2, 2, 2>(_a, _a); }
coFORCE_INLINE coFloatx4 coBroadcastW(const coFloatx4& _a) { return coShuffle<3, 3, 3, 3>(_a, _a); }
coFORCE_INLINE coFloatx4 coSelect(const coFloatx4& _a, const coFloatx4& _b, const coInt32x4& _mask)
{
	return coBitCast<coFloatx4>(coBitCast<coInt32x4>(_b) ^ _mask & (coBitCast<coInt32x4>(_a) ^ coBitCast<coInt32x4>(_b)));
}
coFORCE_INLINE coFloatx4 coSelectX(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_X, coBitCast<__m128>(_a)), _mm_andnot_ps(__m128_MASK_X, coBitCast<__m128>(_b))));
}
coFORCE_INLINE coFloatx4 coSelectY(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_Y, coBitCast<__m128>(_a)), _mm_andnot_ps(__m128_MASK_Y, coBitCast<__m128>(_b))));
}
coFORCE_INLINE coFloatx4 coSelectZ(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_Z, coBitCast<__m128>(_a)), _mm_andnot_ps(__m128_MASK_Z, coBitCast<__m128>(_b))));
}
coFORCE_INLINE coFloatx4 coSelectXYZ(const coFloatx4& _a, const coFloatx4& _b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_and_ps(__m128_MASK_XYZ, coBitCast<__m128>(_a)), _mm_andnot_ps(__m128_MASK_XYZ, coBitCast<__m128>(_b))));
}
coFORCE_INLINE coFloatx4 coMake_floatx4XYZ0(const coFloatx4& _xyz)
{
	return coBitCast<coFloatx4>(_mm_and_ps(coBitCast<__m128>(_xyz), coBitCast<__m128>(__m128_MASK_XYZ)));
}
coFORCE_INLINE coFloatx4 coMake_floatx4XYZ0(const coFloatx3& _xyz)
{
	return coBitCast<coFloatx4>(_mm_and_ps(coBitCast<__m128>(_xyz), coBitCast<__m128>(__m128_MASK_XYZ)));
}
coFORCE_INLINE coFloatx4 coAbs(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_andnot_ps(__m128_SIGN_MASK, coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coSquare(const coFloatx4& a) { return a * a; }
coFORCE_INLINE coFloatx4 coSquareRoot(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_sqrt_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coInvert(const coFloatx4& _a) { return coFloatx4_ONE / _a; }
coFORCE_INLINE coFloatx4 coInvertApprox(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_rcp_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coInvSquareRoot(const coFloatx4& _a)
{
	// newton-raphson (impl found in Vectormath)
	const __m128 a = coBitCast<__m128>(_a);
	const __m128 approx = _mm_rsqrt_ps(a);
	const __m128 muls = _mm_mul_ps(_mm_mul_ps(a, approx), approx);
	const __m128 half4 = _mm_set_ps1(0.5f);
	const __m128 three = _mm_set_ps1(3.0f);
	return coBitCast<coFloatx4>(_mm_mul_ps(_mm_mul_ps(half4, approx), _mm_sub_ps(three, muls)));
}
coFORCE_INLINE coFloatx4 coInvSqrtFast(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_rsqrt_ps(coBitCast<__m128>(_a))); }
coFORCE_INLINE coFloatx4 coTruncate(const coFloatx4& _a)
{
	return coBitCast<coFloatx4>(_mm_cvtepi32_ps(_mm_cvttps_epi32(coBitCast<__m128>(_a))));
}
coFORCE_INLINE coFloatx4 coFloor(const coFloatx4& _a)
{
	return coBitCast<coFloatx4>(_mm_floor_ps(coBitCast<__m128>(_a)));
}
coFORCE_INLINE coFloatx4 coCeil(const coFloatx4& _a)
{
	return coBitCast<coFloatx4>(_mm_ceil_ps(coBitCast<__m128>(_a)));
}
coFORCE_INLINE coFloatx4 coSign(const coFloatx4& x) { return coSelect(1.0f, -1.0f, coBitCast<coInt32x4>(x >= 0.f)); }
coFORCE_INLINE coFloatx4 coPow2(const coFloatx4& _a) { return  _a * _a; }
coFORCE_INLINE coFloatx4 coPow4(const coFloatx4& _a) { return coPow2(_a * _a); }
coFORCE_INLINE coFloatx4 coDenullify(const coFloatx4& _a) { return _a + coMake_floatx4(+1.0e-037f); }
coFORCE_INLINE coFloatx4 coMin(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_min_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 coMin(const coFloatx4& a) { return coMin(coMin(coBroadcastX(a), coBroadcastY(a)), coMin(coBroadcastZ(a), coBroadcastW(a))); }
coFORCE_INLINE coFloatx4 coMax(const coFloatx4& _a, const coFloatx4& _b) { return coBitCast<coFloatx4>(_mm_max_ps(coBitCast<__m128>(_a), coBitCast<__m128>(_b))); }
coFORCE_INLINE coFloatx4 coMax(const coFloatx4& a) { return coMax(coMax(coBroadcastX(a), coBroadcastY(a)), coMax(coBroadcastZ(a), coBroadcastW(a))); }
coFORCE_INLINE coFloatx4 coMaxWith0(const coFloatx4& _a) { return coBitCast<coFloatx4>(_mm_max_ps(coBitCast<__m128>(_a), _mm_setzero_ps())); }
coFORCE_INLINE coFloatx4 coClamp(const coFloatx4& _a, const coFloatx4& _min, const coFloatx4& _max) { return coMin(coMax(_a, _min), _max); }
coFORCE_INLINE coFloatx4 coClamp01(const coFloatx4& _a) { return coClamp(_a, coFloatx4_ZERO, coFloatx4_ONE); }
coFORCE_INLINE coBool32x4 coNearEqual(const coFloatx4& _a, const coFloatx4& _b, const coFloatx4& _epsilon = coFloatx4(coFloat_NearEqualDefaultEpsilon)) { return coAbs(_b - _a) < _epsilon; }
coFORCE_INLINE coBool32x4 coNearEqual0(const coFloatx4& _a, const coFloatx4& _epsilon = coFloatx4(coFloat_NearEqualDefaultEpsilon)) { return coAbs(_a) < _epsilon; }
coFORCE_INLINE coBool32x4 coNotNearEqual0(const coFloatx4& _a, const coFloatx4& _epsilon = coFloatx4(coFloat_NearEqualDefaultEpsilon)) { const coFloatx4 abs = coAbs(_a); return coAbs(_a) > _epsilon; }
coFORCE_INLINE coBool32x4 coAreXYZWEqual(const coFloatx4& _a) { const coFloatx4 x = coBroadcastX(_a); return x == coBroadcastY(_a) && x == coBroadcastZ(_a) && x == coBroadcastW(_a); }

namespace _coFloatx4
{
	const coFloat sinConstantP = 0.225f;
	const coFloatx4 sinConstantA = (16.0f * coSquareRoot(sinConstantP));
	const coFloatx4 sinConstantB = ((1.0f - sinConstantP) * coInvSquareRoot(sinConstantP));
};
coFORCE_INLINE coFloatx4 coSin(const coFloatx4& _a)
{
	/*
	// From V4Sin from PhysX 4.1
	const Vec4V recipTwoPi = V4LoadA(g_PXReciprocalTwoPi.f);
	const Vec4V twoPi = V4LoadA(g_PXTwoPi.f);
	const Vec4V tmp = V4Mul(a, recipTwoPi);
	const Vec4V b = V4Round(tmp);
	const Vec4V V1 = V4NegMulSub(twoPi, b, a);

	// sin(V) ~= V - V^3 / 3! + V^5 / 5! - V^7 / 7! + V^9 / 9! - V^11 / 11! + V^13 / 13! -
	//           V^15 / 15! + V^17 / 17! - V^19 / 19! + V^21 / 21! - V^23 / 23! (for -PI <= V < PI)
	const coFloatx4 V2 = V1 * V1;
	const coFloatx4 V3 = V2 * V1;
	const coFloatx4 V5 = V3 * V2;
	const coFloatx4 V7 = V5 * V2;
	const coFloatx4 V9 = V7 * V2;
	const coFloatx4 V11 = V9 * V2;
	const coFloatx4 V13 = V11 * V2;
	const coFloatx4 V15 = V13 * V2;
	const coFloatx4 V17 = V15 * V2;
	const coFloatx4 V19 = V17 * V2;
	const coFloatx4 V21 = V19 * V2;
	const coFloatx4 V23 = V21 * V2;

	const Vec4V sinCoefficients0 = V4LoadA(g_PXSinCoefficients0.f);
	const Vec4V sinCoefficients1 = V4LoadA(g_PXSinCoefficients1.f);
	const Vec4V sinCoefficients2 = V4LoadA(g_PXSinCoefficients2.f);

	const coFloatx4 S1 = coBroadcastY(sinCoefficients0);
	const coFloatx4 S2 = coBroadcastZ(sinCoefficients0);
	const coFloatx4 S3 = coBroadcastW(sinCoefficients0);
	const coFloatx4 S4 = coBroadcastX(sinCoefficients1);
	const coFloatx4 S5 = coBroadcastY(sinCoefficients1);
	const coFloatx4 S6 = coBroadcastZ(sinCoefficients1);
	const coFloatx4 S7 = coBroadcastW(sinCoefficients1);
	const coFloatx4 S8 = coBroadcastX(sinCoefficients2);
	const coFloatx4 S9 = coBroadcastY(sinCoefficients2);
	const coFloatx4 S10 = coBroadcastZ(sinCoefficients2);
	const coFloatx4 S11 = coBroadcastW(sinCoefficients2);

	coFloatx4 result;
	result = coMulAdd(S1, V3, V1);
	result = coMulAdd(S2, V5, result);
	result = coMulAdd(S3, V7, result);
	Result = coMulAdd(S4, V9, result);
	result = coMulAdd(S5, V11, result);
	result = coMulAdd(S6, V13, result);
	result = coMulAdd(S7, V15, result);
	result = coMulAdd(S8, V17, result);
	result = coMulAdd(S9, V19, result);
	result = coMulAdd(S10, V21, result);
	result = coMulAdd(S11, V23, result);
	return result;*/
	// Based on: http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
	coFloatx4 y = _a * coFloatx4_::inv2Pi;
	y = y - coFloor(y + coFloatx4(0.5f));
	y = _coFloatx4::sinConstantA * y * (coFloatx4(0.5f) - coAbs(y));
	return y * (_coFloatx4::sinConstantB + coAbs(y));
}
coFORCE_INLINE coFloatx4 coSinApprox(const coFloatx4& _a)
{
	return coSin(_a);
}
coFORCE_INLINE coFloatx4 coCos(const coFloatx4& _a)
{
	return coSin(_a + coFloatx4_::halfPi);
}
coFORCE_INLINE coFloatx4 coCosApprox(const coFloatx4& _a)
{
	return coSinApprox(_a + coFloatx4_::halfPi);
}

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coFloatx4& a)
{
	stream.Write(&a, 16);
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coFloatx4& a)
{
	stream.Read(&a, 16);
	return stream;
}

coFORCE_INLINE void coFloatx4::Write(coBinaryOutputStream& stream) const
{
	stream.Write(this, 16);
}

coFORCE_INLINE void coFloatx4::Read(coBinaryInputStream& stream)
{
	stream.Read(this, 16);
}
