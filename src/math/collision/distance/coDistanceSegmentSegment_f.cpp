// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDistanceSegmentSegment_f.h"

coFORCE_INLINE coVec4 V4UnpackXY(const coVec4 a, const coVec4 b)
{
	return coBitCast<coVec4>(_mm_unpacklo_ps(coBitCast<__m128>(a), coBitCast<__m128>(b)));
}

coFORCE_INLINE coVec4 V4UnpackZW(const coVec4 a, const coVec4 b)
{
	return coBitCast<coVec4>(_mm_unpackhi_ps(coBitCast<__m128>(a), coBitCast<__m128>(b)));
}

// this macro transposes 4 Vec4V into 3 Vec4V (assuming that the W component can be ignored
#define coTRANSPOSE_44_34(inA, inB, inC, inD, outA, outB, outC)\
	\
outA = V4UnpackXY(inA, inC);\
	\
inA = V4UnpackZW(inA, inC);\
	\
inC = V4UnpackXY(inB, inD);\
	\
inB = V4UnpackZW(inB, inD);\
	\
outB = V4UnpackZW(outA, inC);\
	\
outA = V4UnpackXY(outA, inC);\
	\
outC = V4UnpackXY(inA, inB);

coFORCE_INLINE coVec4 V4Sel(const coBool32x4 c, const coVec4 a, const coVec4 b)
{
	return coBitCast<coVec4>(_mm_or_ps(_mm_andnot_ps(coBitCast<__m128>(c), coBitCast<__m128>(b)), _mm_and_ps(coBitCast<__m128>(c), coBitCast<__m128>(a))));
}

/*
	segment (p, d) and segment (p02, d02)
	segment (p, d) and segment (p12, d12)
	segment (p, d) and segment (p22, d22)
	segment (p, d) and segment (p32, d32)
*/
coVec4 coDistanceSegmentSegmentSquared4(const coVec3 p, const coVec3 d0,
	const coVec3 p02, const coVec3 d02,
	const coVec3 p12, const coVec3 d12,
	const coVec3 p22, const coVec3 d22,
	const coVec3 p32, const coVec3 d32,
	coVec4& s, coVec4& t)
{
	//=====
	// Impl from distanceSegmentSegmentSquared4(), \PhysX\physx\source\geomutils\src\distance\GuDistanceSegmentSegment.cpp, PhysX 4
	// Most comments are from the original code.
	// From the open source version of the PhysX SDK
	//=====
	const coVec4 zero = coVec4(0.0f);
	const coVec4 one = coVec4(1.0f);
	const coVec4 eps = coVec4(FLT_EPSILON);
	const coVec4 half = coVec4(0.5f);

	const coVec4 d0X = coBroadcastX(coFloatx4(d0));
	const coVec4 d0Y = coBroadcastY(coFloatx4(d0));
	const coVec4 d0Z = coBroadcastZ(coFloatx4(d0));
	const coVec4 pX = coBroadcastX(coFloatx4(p));
	const coVec4 pY = coBroadcastY(coFloatx4(p));
	const coVec4 pZ = coBroadcastZ(coFloatx4(p));

	coVec4 d024 = coVec4(d02);
	coVec4 d124 = coVec4(d12);
	coVec4 d224 = coVec4(d22);
	coVec4 d324 = coVec4(d32);

	coVec4 p024 = coVec4(p02);
	coVec4 p124 = coVec4(p12);
	coVec4 p224 = coVec4(p22);
	coVec4 p324 = coVec4(p32);

	coVec4 d0123X, d0123Y, d0123Z;
	coVec4 p0123X, p0123Y, p0123Z;

	coTRANSPOSE_44_34(d024, d124, d224, d324, d0123X, d0123Y, d0123Z);
	coTRANSPOSE_44_34(p024, p124, p224, p324, p0123X, p0123Y, p0123Z);

	const coVec4 rX = pX - p0123X;
	const coVec4 rY = pY - p0123Y;
	const coVec4 rZ = pZ - p0123Z;

	//TODO - store this in a transposed state and avoid so many dot products?

	const coFloatx4 dd = coDot(d0, d0);

	const coVec4 e = coMulAdd(d0123Z, d0123Z, coMulAdd(d0123X, d0123X, d0123Y * d0123Y));
	const coVec4 b = coMulAdd(d0Z, d0123Z, coMulAdd(d0X, d0123X, d0Y * d0123Y));
	const coVec4 c = coMulAdd(d0Z, rZ, coMulAdd(d0X, rX, d0Y * rY));
	const coVec4 f = coMulAdd(d0123Z, rZ, coMulAdd(d0123X, rX, d0123Y * rY));

	const coVec4 a(dd);

	const coVec4 aRecip(coInvert(a));
	const coVec4 eRecip(coInvert(e));

	//if segments not parallell, compute closest point on two segments and clamp to segment1
	const coVec4 denom = a * e - b * b;
	const coVec4 temp = b * f - c * e;
	const coVec4 s0 = coClamp(temp / denom, zero, one);

	//test whether segments are parallel
	const coBool32x4 con2 = eps >= denom;
	const coVec4 sTmp = V4Sel(con2, half, s0);

	//compute point on segment2 closest to segment1
	const coVec4 tTmp = (b * sTmp + f) * eRecip;

	//if t is in [zero, one], done. otherwise clamp t
	const coVec4 t2 = coClamp(tTmp, zero, one);

	//recompute s for the new value
	const coVec4 comp = (b * t2 - c) * aRecip;
	const coBool32x4 aaNearZero = eps >= a; // check if aRecip is valid (aa>eps)
	const coVec4 s2 = V4Sel(aaNearZero, coVec4(0.0f), coClamp(comp, zero, one));

	/*  s = V4Sel(con0, zero, V4Sel(con1, cd, s2));
	t = V4Sel(con1, zero, V4Sel(con0, cg, t2));  */
	s = s2;
	t = t2;

	const coVec4 closest1X = coMulAdd(d0X, s2, pX);
	const coVec4 closest1Y = coMulAdd(d0Y, s2, pY);
	const coVec4 closest1Z = coMulAdd(d0Z, s2, pZ);

	const coVec4 closest2X = coMulAdd(d0123X, t2, p0123X);
	const coVec4 closest2Y = coMulAdd(d0123Y, t2, p0123Y);
	const coVec4 closest2Z = coMulAdd(d0123Z, t2, p0123Z);

	const coVec4 vvX = closest1X - closest2X;
	const coVec4 vvY = closest1Y - closest2Y;
	const coVec4 vvZ = closest1Z - closest2Z;

	const coVec4 vd = coMulAdd(vvX, vvX, coMulAdd(vvY, vvY, vvZ * vvZ));

	return vd;
}
