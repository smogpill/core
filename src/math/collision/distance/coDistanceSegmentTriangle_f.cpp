// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDistanceSegmentTriangle_f.h"
#include "coDistanceSegmentSegment_f.h"
#include "math/shape/coDistance_f.h"

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
	// From the open source version of the PhysX SDK
	//=====
	const coFloatx4 zero = -coFloatx4(FLT_EPSILON);
	const coFloatx4 one = coFloatx4(1.0f) + coFloatx4(FLT_EPSILON);

	const coBool32x4 con0 = (v >= zero) & (one >= v);
	const coBool32x4 con1 = (w >= zero) & (one >= w);
	const coBool32x4 con2 = one > (v + w);
	return con0 & con1 & con2;
}

/*
	closest0 is the closest point on segment pq
	closest1 is the closest point on triangle abc
*/
coFloatx4 coDistanceSegmentTriangleSquared(const coVec3& p, const coVec3& q,
	const coVec3& a, const coVec3& b, const coVec3& c, coVec3& closest0, coVec3& closest1)
{
	//=====
	// Impl from Gu::distanceSegmentTriangleSquared(), \physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp, PhysX 4
	// Most comments are from the original code.
	// From the open source version of the PhysX SDK
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

	const coVec3 n = coNormalize(coCross(ac, ab)); // normalize vector

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

	const coFloatx4 t00 = coSplatX(t40);
	const coFloatx4 t10 = coSplatY(t40);
	const coFloatx4 t20 = coSplatZ(t40);

	const coFloatx4 t01 = coSplatX(t41);
	const coFloatx4 t11 = coSplatY(t41);
	const coFloatx4 t21 = coSplatZ(t41);

	const coFloatx4 sqDist0(coSplatX(sqDist44));
	const coFloatx4 sqDist1(coSplatY(sqDist44));
	const coFloatx4 sqDist2(coSplatZ(sqDist44));

	const coVec3 closestP00 = coMulAdd(pq, t00, p);
	const coVec3 closestP01 = coMulAdd(ab, t01, a);

	const coVec3 closestP10 = coMulAdd(pq, t10, p);
	const coVec3 closestP11 = coMulAdd(bc, t11, b);

	const coVec3 closestP20 = coMulAdd(pq, t20, p);
	const coVec3 closestP21 = coMulAdd(ac, t21, a);


	//Get the closest point of all edges
	const coBool32x4 con20 = sqDist1 > sqDist0;
	const coBool32x4 con21 = sqDist2 > sqDist0;
	const coBool32x4 con2 = con20 & con21;
	const coBool32x4 con30 = sqDist0 >= sqDist1;
	const coBool32x4 con31 = sqDist2 > sqDist1;
	const coBool32x4 con3 = con30 & con31;
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

	const coBool32x4 cond2 = con0 & con1;

	const coVec3 closestP0 = V3Sel(cond2, c20, V3Sel(con0, c00, V3Sel(con1, c10, closestPE0)));
	const coVec3 closestP1 = V3Sel(cond2, c21, V3Sel(con0, c01, V3Sel(con1, c11, closestPE1)));

	const coVec3 vv = closestP1 - closestP0;
	closest0 = closestP0;
	closest1 = closestP1;
	return coDot(vv, vv);
}

coFloat coSquareDistanceSegmentTriangle(const coVec3& vOrigin, const coVec3& vSegment, const coVec3& vP0, const coVec3& vTriEdge0, const coVec3& vTriEdge1)
{
	//=====
	// Impl from Gu::distanceSegmentTriangleSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp
	// Most comments are from the original code.
	// From the open source version of the PhysX SDK
	//=====

	const coFloat fA00 = coSquareLength(vSegment).x;
	const coVec3 vDiff = vP0 - vOrigin;
	const coFloat fA01 = -(coDot(vSegment, vTriEdge0).x);
	const coFloat fA02 = -(coDot(vSegment, vTriEdge1).x);
	const coFloat fA11 = coSquareLength(vTriEdge0).x;
	const coFloat fA12 = coDot(vTriEdge0, vTriEdge1).x;
	const coFloat fA22 = coDot(vTriEdge1, vTriEdge1).x;
	const coFloat fB0 = -(coDot(vDiff, vSegment).x);
	const coFloat fB1 = coDot(vDiff, vTriEdge0).x;
	const coFloat fB2 = coDot(vDiff, vTriEdge1).x;
	const coFloat fCof00 = fA11 * fA22 - fA12 * fA12;
	const coFloat fCof01 = fA02 * fA12 - fA01 * fA22;
	const coFloat fCof02 = fA01 * fA12 - fA02 * fA11;
	const coFloat fDet = fA00 * fCof00 + fA01 * fCof01 + fA02 * fCof02;

	coFloat fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

	// Set up for a relative error test on the angle between ray direction
	// and triangle normal to determine parallel/nonparallel status.
	const coVec3 vNormal = coCross(vTriEdge1, vTriEdge0);
	const coFloat fDot = coDot(vNormal, vSegment).x;
	if (fDot * fDot >= 1e-6f * coSquareLength(vSegment) * coSquareLength(vNormal))
	{
		const coFloat fCof11 = fA00 * fA22 - fA02 * fA02;
		const coFloat fCof12 = fA02 * fA01 - fA00 * fA12;
		const coFloat fCof22 = fA00 * fA11 - fA01 * fA01;
		const coFloat fInvDet = fDet == 0.0f ? 0.0f : 1.0f / fDet;
		const coFloat fRhs0 = -fB0 * fInvDet;
		const coFloat fRhs1 = -fB1 * fInvDet;
		const coFloat fRhs2 = -fB2 * fInvDet;

		fR = fCof00 * fRhs0 + fCof01 * fRhs1 + fCof02 * fRhs2;
		fS = fCof01 * fRhs0 + fCof11 * fRhs1 + fCof12 * fRhs2;
		fT = fCof02 * fRhs0 + fCof12 * fRhs1 + fCof22 * fRhs2;

		if (fR < 0.0f)
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4m
					{
						// minimum on face s=0 or t=0 or r=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3m
					{
						// minimum on face s=0 or r=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5m
				{
					// minimum on face t=0 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0m
				{
					// minimum on face r=0
					fSqrDist = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 0.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2m
				{
					// minimum on face s=0 or s+t=1 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6m
				{
					// minimum on face t=0 or s+t=1 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1m
				{
					// minimum on face s+t=1 or r=0
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
				fR0 = 0.0f;
				_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
			}
		}
		else if (fR <= 1.0f)
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4
					{
						// minimum on face s=0 or t=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3
					{
						// minimum on face s=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
				}
				else if (fT < 0.0f)  // region 5
				{
					// minimum on face t=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
				}
				else  // region 0
				{
					// global minimum is interior, done
					fSqrDist = fR * (fA00 * fR + fA01 * fS + fA02 * fT + 2.0f * fB0)
						+ fS * (fA01 * fR + fA11 * fS + fA12 * fT + 2.0f * fB1)
						+ fT * (fA02 * fR + fA12 * fS + fA22 * fT + 2.0f * fB2)
						+ coSquareLength(vDiff).x;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2
				{
					// minimum on face s=0 or s+t=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6
				{
					// minimum on face t=0 or s+t=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1
				{
					// minimum on face s+t=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
			}
		}
		else  // fR > 1
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4p
					{
						// minimum on face s=0 or t=0 or r=1
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3p
					{
						// minimum on face s=0 or r=1
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5p
				{
					// minimum on face t=0 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;

					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0p
				{
					// minimum face on r=1
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 1.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2p
				{
					// minimum on face s=0 or s+t=1 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6p
				{
					// minimum on face t=0 or s+t=1 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1p
				{
					// minimum on face s+t=1 or r=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				const coVec3 kPt = vOrigin + vSegment;
				fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
				fR0 = 1.0f;
				_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
			}
		}
	}
	else
	{
		// Segment and triangle are parallel
		fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
		fT = 0.0f;

		fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR0, &fT0);
		fS0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kTriSegOrig = vP0 + vTriEdge0;
		const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
		fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
		fS0 = 1.0f - fT0;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kPt = vOrigin + vSegment;
		fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 1.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
	}

	// Account for numerical round-off error
	return coMax(0.0f, fSqrDist);
}
