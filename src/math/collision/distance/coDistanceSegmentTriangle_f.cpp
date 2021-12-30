// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDistanceSegmentTriangle_f.h"
#include "coDistanceSegmentSegment_f.h"

coFORCE_INLINE coFloatx4 FSel(const coBool32x4 c, const coFloatx4 a, const coFloatx4 b)
{
	return coBitCast<coFloatx4>(_mm_or_ps(_mm_andnot_ps(coBitCast<__m128>(c), coBitCast<__m128>(b)), _mm_and_ps(coBitCast<__m128>(c), coBitCast<__m128>(a))));
}

coFORCE_INLINE coVec3 V3Sel(const coBool32x4 c, const coVec3 a, const coVec3 b)
{
	return coBitCast<coVec3>(_mm_or_ps(_mm_andnot_ps(coBitCast<__m128>(c), coBitCast<__m128>(b)), _mm_and_ps(coBitCast<__m128>(c), coBitCast<__m128>(a))));
}

coFORCE_INLINE coBool32x4 coIsValidTriangleBarycentricCoord(const coFloatx4 v, const coFloatx4 w)
{
	//=====
	// Impl from isValidTriangleBarycentricCoord(), \PhysX\physx\source\geomutils\src\common\GuBarycentricCoordinates.h, PhysX 4
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====
	const coFloatx4 zero = -coFloatx4(FLT_EPSILON);
	const coFloatx4 one = coFloatx4(1.0f) + coFloatx4(FLT_EPSILON);

	const coBool32x4 con0 = (v >= zero) && (one >= v);
	const coBool32x4 con1 = (w >= zero) && (one >= w);
	const coBool32x4 con2 = one > (v + w);
	return con0 && con1 && con2;
}

/*
	closest0 is the closest point on segment pq
	closest1 is the closest point on triangle abc
*/
coFloatx4 coDistanceSegmentTriangleSquared(const coVec3 p, const coVec3 q,
	const coVec3 a, const coVec3 b, const coVec3 c, coVec3& closest0, coVec3& closest1)
{
	//=====
	// Impl from Gu::distanceSegmentTriangleSquared(), \physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp, PhysX 4
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	const coFloatx4 zero = coFloatx4_ZERO;
	//const FloatV one = FOne();
	//const FloatV parallelTolerance  = FloatV_From_F32(PX_PARALLEL_TOLERANCE);

	const coVec3 pq = q - p;
	const coVec3 ab = b - a;
	const coVec3 ac = c - a;
	const coVec3 bc = c - b;
	const coVec3 ap = p - a;
	const coVec3 aq = q - a;

	//This is used to calculate the barycentric coordinate
	const coFloatx4 d00 = coDot(ab, ab);
	const coFloatx4 d01 = coDot(ab, ac);
	const coFloatx4 d11 = coDot(ac, ac);
	const coFloatx4 tDenom = d00 * d11 - d01 * d01;

	const coFloatx4 bdenom = FSel(tDenom > zero, coInvert(tDenom), zero);

	const coVec3 n = coNormalize(coCross(ab, ac)); // normalize vector

	//compute the closest point of p and triangle plane abc
	const coFloatx4 dist3 = coDot(ap, n);
	const coFloatx4 sqDist3 = dist3 * dist3;


	//compute the closest point of q and triangle plane abc
	const coFloatx4 dist4 = coDot(aq, n);
	const coFloatx4 sqDist4 = dist4 * dist4;
	const coFloatx4 dMul = dist3 * dist4;
	const coBool32x4 con = zero > dMul;


	// intersect with the plane
	if (coAreAllTrue(con))
	{
		//compute the intersect point
		const coFloatx4 nom = -coDot(n, ap);
		const coFloatx4 denom = coInvert(coDot(n, pq));
		const coFloatx4 t = nom * denom;
		const coVec3 ip = coMulAdd(pq, coVec3(t), p);//V3Add(p, V3Scale(pq, t));
		const coVec3 v2 = ip - a;
		const coFloatx4 d20 = coDot(v2, ab);
		const coFloatx4 d21 = coDot(v2, ac);
		const coFloatx4 v0 = (d11 * d20 - d01 * d21) * bdenom;
		const coFloatx4 w0 = (d00 * d21 - d01 * d20) * bdenom;
		const coBool32x4 con0 = coIsValidTriangleBarycentricCoord(v0, w0);
		if (coAreAllTrue(con0))
		{
			closest0 = closest1 = ip;
			return zero;
		}
	}


	coVec4 t40, t41;
	const coVec4 sqDist44 = coDistanceSegmentSegmentSquared4(p, pq, a, ab, b, bc, a, ac, a, ab, t40, t41);

	const coFloatx4 t00 = coBroadcastX(t40);
	const coFloatx4 t10 = coBroadcastY(t40);
	const coFloatx4 t20 = coBroadcastZ(t40);

	const coFloatx4 t01 = coBroadcastX(t41);
	const coFloatx4 t11 = coBroadcastY(t41);
	const coFloatx4 t21 = coBroadcastZ(t41);

	const coFloatx4 sqDist0(coBroadcastX(sqDist44));
	const coFloatx4 sqDist1(coBroadcastY(sqDist44));
	const coFloatx4 sqDist2(coBroadcastZ(sqDist44));

	const coVec3 closestP00 = coMulAdd(pq, t00, p);
	const coVec3 closestP01 = coMulAdd(ab, t01, a);

	const coVec3 closestP10 = coMulAdd(pq, t10, p);
	const coVec3 closestP11 = coMulAdd(bc, t11, b);

	const coVec3 closestP20 = coMulAdd(pq, t20, p);
	const coVec3 closestP21 = coMulAdd(ac, t21, a);


	//Get the closest point of all edges
	const coBool32x4 con20 = sqDist1 > sqDist0;
	const coBool32x4 con21 = sqDist2 > sqDist0;
	const coBool32x4 con2 = con20 && con21;
	const coBool32x4 con30 = sqDist0 >= sqDist1;
	const coBool32x4 con31 = sqDist2 > sqDist1;
	const coBool32x4 con3 = con30 && con31;
	const coFloatx4 sqDistPE = FSel(con2, sqDist0, FSel(con3, sqDist1, sqDist2));
	//const FloatV tValue = FSel(con2, t00, FSel(con3, t10, t20));
	const coVec3 closestPE0 = V3Sel(con2, closestP00, V3Sel(con3, closestP10, closestP20)); // closestP on segment
	const coVec3 closestPE1 = V3Sel(con2, closestP01, V3Sel(con3, closestP11, closestP21)); // closestP on triangle


	const coVec3 closestP31 = p - n * dist3;
	const coVec3 closestP30 = p;

	//Compute the barycentric coordinate for project point of q
	const coVec3 pV20 = closestP31 - a;
	const coFloatx4 pD20 = coDot(pV20, ab);
	const coFloatx4 pD21 = coDot(pV20, ac);
	const coFloatx4 v0 = (d11 * pD20 - d01 * pD21) * bdenom;
	const coFloatx4 w0 = (d00 * pD21 - d01 * pD20) * bdenom;

	//check closestP3 is inside the triangle
	const coBool32x4 con0 = coIsValidTriangleBarycentricCoord(v0, w0);

	const coVec3 closestP41 = q - n * dist4;
	const coVec3 closestP40 = q;

	//Compute the barycentric coordinate for project point of q
	const coVec3 qV20 = closestP41 - a;
	const coFloatx4 qD20 = coDot(qV20, ab);
	const coFloatx4 qD21 = coDot(qV20, ac);
	const coFloatx4 v1 = (d11 * qD20 - d01 * qD21) * bdenom;
	const coFloatx4 w1 = (d00 * qD21 - d01 * qD20) * bdenom;

	const coBool32x4 con1 = coIsValidTriangleBarycentricCoord(v1, w1);

	/*
		p is interior point but not q
	*/
	const coBool32x4 d0 = sqDistPE > sqDist3;
	const coVec3 c00 = V3Sel(d0, closestP30, closestPE0);
	const coVec3 c01 = V3Sel(d0, closestP31, closestPE1);

	/*
		q is interior point but not p
	*/
	const coBool32x4 d1 = sqDistPE > sqDist4;
	const coVec3 c10 = V3Sel(d1, closestP40, closestPE0);
	const coVec3 c11 = V3Sel(d1, closestP41, closestPE1);

	/*
		p and q are interior point
	*/
	const coBool32x4 d2 = sqDist4 > sqDist3;
	const coVec3 c20 = V3Sel(d2, closestP30, closestP40);
	const coVec3 c21 = V3Sel(d2, closestP31, closestP41);

	const coBool32x4 cond2 = con0 && con1;

	const coVec3 closestP0 = V3Sel(cond2, c20, V3Sel(con0, c00, V3Sel(con1, c10, closestPE0)));
	const coVec3 closestP1 = V3Sel(cond2, c21, V3Sel(con0, c01, V3Sel(con1, c11, closestPE1)));

	const coVec3 vv = closestP1 - closestP0;
	closest0 = closestP0;
	closest1 = closestP1;
	return coDot(vv, vv);
}
