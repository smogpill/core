// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3_f.h"
#include "coAabb.h"
#include "coSphere.h"
#include "coIntersection_f.h"

inline coFloatx4 coSquareDistancePointSegment(const coVec3& p, const coVec3& a, const coVec3& b)
{
	const coVec3 ab = b - a;
	const coVec3 ap = p - a;
	const coVec3 bp = p - b;
	const coFloatx4 e = coDot(ap, ab);
	if (e <= 0.0f)
		return coSquareLength(ap);
	const coFloatx4 f = coSquareLength(ab);
	if (e >= f)
		return coSquareLength(bp);
	return coSquareLength(ap) - e * e / f;
}

coFORCE_INLINE coFloat coSquareDistancePointSegment2(const coVec3& p0, const coVec3& dir, const coVec3& point, coFloat* param = nullptr)
{
	coVec3 diff = point - p0;
	coFloat fT = coDot(diff, dir).x;

	if (fT <= 0.0f)
		fT = 0.0f;
	else
	{
		const coFloat sqrLen = coSquareLength(dir).x;
		if (fT >= sqrLen)
		{
			fT = 1.0f;
			diff -= dir;
		}
		else
		{
			fT /= sqrLen;
			diff -= fT * dir;
		}
	}

	if (param)
		*param = fT;

	return coSquareLength(diff).x;
}

inline coFloatx4 coDistancePointSegment(const coVec3& p, const coVec3& a, const coVec3& b)
{
	return coSquareRoot(coSquareDistancePointSegment(p, a, b));
}

inline coFloatx4 coSquareDistanceSegmentSegment(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& d)
{
	coFloat t;
	if (coIntersectSegmentSegment(a, b, c, d, t))
		return 0.0f;

	coFloatx4 minSquareDist = coSquareDistancePointSegment(a, c, d);
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(b, c, d));
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(c, a, b));
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(d, a, b));
	return minSquareDist;
}

inline coFloatx4 coSquareDistance(const coAabb& a, const coAabb& b)
{
	coAabb outer;
	outer.min = coMin(a.min, b.min);
	outer.max = coMax(a.max, b.max);
	const coVec3 inner = coMax(0.0f, (outer.max - outer.min) - (a.max - a.min) - (b.max - b.min));
	return coSquareLength(inner);
}

coFORCE_INLINE coFloatx4 coDistance(const coAabb& a, const coVec3& p)
{
	const coVec3 center = (a.max + a.min) * 0.5f;
	const coVec3 d = coAbs(p - center) - (a.max - a.min) * 0.5f;
	return coLength(coMax(d, 0.0f)) + coMin(coBitCast<coFloatx4>(coMax(d)), 0.0f);
}

coFORCE_INLINE coFloatx4 coDistance(const coAabb& a, const coSphere& s)
{
	return coDistance(a, s.centerAndRadius) - coBroadcastW(s.centerAndRadius);
}

inline coFloatx4 coSquareDistancePointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c)
{
	// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
	const coVec3 ba = b - a; const coVec3 pa = p - a;
	const coVec3 cb = c - b; const coVec3 pb = p - b;
	const coVec3 ac = a - c; const coVec3 pc = p - c;
	const coVec3 nor = coCross(ba, ac);

	const coFloatx4 f = coSign(coDot(coCross(ba, nor), pa))
		+ coSign(coDot(coCross(cb, nor), pb))
		+ coSign(coDot(coCross(ac, nor), pc));
	const coFloatx4 v = f < 2.0f ?
		coMin(coMin(
			coSquareLength(ba * coClamp01(coDot(ba, pa) / coSquareLength(ba)) - pa),
			coSquareLength(cb * coClamp01(coDot(cb, pb) / coSquareLength(cb)) - pb)),
			coSquareLength(ac * coClamp01(coDot(ac, pc) / coSquareLength(ac)) - pc))
		: coDot(nor, pa) * coDot(nor, pa) / coSquareLength(nor);
	return v;
}

inline coFloatx4 coSquareDistancePointQuad(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& d)
{
	// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
	const coVec3 ba = b - a; const coVec3 pa = p - a;
	const coVec3 cb = c - b; const coVec3 pb = p - b;
	const coVec3 dc = d - c; const coVec3 pc = p - c;
	const coVec3 ad = a - d; const coVec3 pd = p - d;
	const coVec3 nor = coCross(ba, ad);

	const coFloatx4 f = coSign(coDot(coCross(ba, nor), pa))
		+ coSign(coDot(coCross(cb, nor), pb))
		+ coSign(coDot(coCross(dc, nor), pc))
		+ coSign(coDot(coCross(ad, nor), pd));
	const coFloatx4 v = f < 3.0f ?
		coMin(coMin(coMin(
			coSquareLength(ba * coClamp01(coDot(ba, pa) / coSquareLength(ba)) - pa),
			coSquareLength(cb * coClamp01(coDot(cb, pb) / coSquareLength(cb)) - pb)),
			coSquareLength(dc * coClamp01(coDot(dc, pc) / coSquareLength(dc)) - pc)),
			coSquareLength(ad * coClamp01(coDot(ad, pd) / coSquareLength(ad)) - pd))
		: coDot(nor, pa) * coDot(nor, pa) / coSquareLength(nor);
	return v;
}

coFORCE_INLINE void _coUpdateClosestHit(coFloat fSqrDist0, coFloat fR0, coFloat fS0, coFloat fT0, coFloat& fSqrDist, coFloat& fR, coFloat& fS, coFloat& fT)
{
	//=====
	// Impl from updateClosestHit(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	if (fSqrDist0 < fSqrDist)
	{
		fSqrDist = fSqrDist0;
		fR = fR0;
		fS = fS0;
		fT = fT0;
	}
}

inline coFloat coSquareDistanceSegmentSegment2(const coVec3& vOrigin0, const coVec3& vDir0, coFloat fExtent0,
	const coVec3& vOrigin1, const coVec3& vDir1, coFloat fExtent1, coFloat* pParam0 = nullptr, coFloat* pParam1 = nullptr)
{
	// S0 = origin + extent * vDir;
	// S1 = origin - extent * vDir;

	//=====
	// Impl from Gu::distanceSegmentSegmentSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentSegment.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	const coVec3 vDiff = vOrigin0 - vOrigin1;
	const coFloat fA01 = -coDot(vDir0, vDir1).x;
	const coFloat fB0 = coDot(vDiff, vDir0).x;
	const coFloat fB1 = -coDot(vDiff, vDir1).x;
	const coFloat fC = coSquareLength(vDiff).x;
	const coFloat fDet = coAbs(1.0f - fA01 * fA01);
	coFloat fS0, fS1, fSqrDist, fExtDet0, fExtDet1, fTmpS0, fTmpS1;

	if (fDet >= 1e-06f)
	{
		// segments are not parallel
		fS0 = fA01 * fB1 - fB0;
		fS1 = fA01 * fB0 - fB1;
		fExtDet0 = fExtent0 * fDet;
		fExtDet1 = fExtent1 * fDet;

		if (fS0 >= -fExtDet0)
		{
			if (fS0 <= fExtDet0)
			{
				if (fS1 >= -fExtDet1)
				{
					if (fS1 <= fExtDet1)  // region 0 (interior)
					{
						// minimum at two interior points of 3D lines
						coFloat fInvDet = 1.0f / fDet;
						fS0 *= fInvDet;
						fS1 *= fInvDet;
						fSqrDist = fS0 * (fS0 + fA01 * fS1 + 2.0f * fB0) + fS1 * (fA01 * fS0 + fS1 + 2.0f * fB1) + fC;
					}
					else  // region 3 (side)
					{
						fS1 = fExtent1;
						fTmpS0 = -(fA01 * fS1 + fB0);
						if (fTmpS0 < -fExtent0)
						{
							fS0 = -fExtent0;
							fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
						else if (fTmpS0 <= fExtent0)
						{
							fS0 = fTmpS0;
							fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
						else
						{
							fS0 = fExtent0;
							fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
					}
				}
				else  // region 7 (side)
				{
					fS1 = -fExtent1;
					fTmpS0 = -(fA01 * fS1 + fB0);
					if (fTmpS0 < -fExtent0)
					{
						fS0 = -fExtent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else if (fTmpS0 <= fExtent0)
					{
						fS0 = fTmpS0;
						fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else
					{
						fS0 = fExtent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
				}
			}
			else
			{
				if (fS1 >= -fExtDet1)
				{
					if (fS1 <= fExtDet1)  // region 1 (side)
					{
						fS0 = fExtent0;
						fTmpS1 = -(fA01 * fS0 + fB1);
						if (fTmpS1 < -fExtent1)
						{
							fS1 = -fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else if (fTmpS1 <= fExtent1)
						{
							fS1 = fTmpS1;
							fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else
						{
							fS1 = fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
					}
					else  // region 2 (corner)
					{
						fS1 = fExtent1;
						fTmpS0 = -(fA01 * fS1 + fB0);
						if (fTmpS0 < -fExtent0)
						{
							fS0 = -fExtent0;
							fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
						else if (fTmpS0 <= fExtent0)
						{
							fS0 = fTmpS0;
							fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
						}
						else
						{
							fS0 = fExtent0;
							fTmpS1 = -(fA01 * fS0 + fB1);
							if (fTmpS1 < -fExtent1)
							{
								fS1 = -fExtent1;
								fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
							}
							else if (fTmpS1 <= fExtent1)
							{
								fS1 = fTmpS1;
								fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
							}
							else
							{
								fS1 = fExtent1;
								fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
							}
						}
					}
				}
				else  // region 8 (corner)
				{
					fS1 = -fExtent1;
					fTmpS0 = -(fA01 * fS1 + fB0);
					if (fTmpS0 < -fExtent0)
					{
						fS0 = -fExtent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else if (fTmpS0 <= fExtent0)
					{
						fS0 = fTmpS0;
						fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else
					{
						fS0 = fExtent0;
						fTmpS1 = -(fA01 * fS0 + fB1);
						if (fTmpS1 > fExtent1)
						{
							fS1 = fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else if (fTmpS1 >= -fExtent1)
						{
							fS1 = fTmpS1;
							fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else
						{
							fS1 = -fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
					}
				}
			}
		}
		else
		{
			if (fS1 >= -fExtDet1)
			{
				if (fS1 <= fExtDet1)  // region 5 (side)
				{
					fS0 = -fExtent0;
					fTmpS1 = -(fA01 * fS0 + fB1);
					if (fTmpS1 < -fExtent1)
					{
						fS1 = -fExtent1;
						fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
					else if (fTmpS1 <= fExtent1)
					{
						fS1 = fTmpS1;
						fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
					else
					{
						fS1 = fExtent1;
						fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
				}
				else  // region 4 (corner)
				{
					fS1 = fExtent1;
					fTmpS0 = -(fA01 * fS1 + fB0);
					if (fTmpS0 > fExtent0)
					{
						fS0 = fExtent0;
						fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else if (fTmpS0 >= -fExtent0)
					{
						fS0 = fTmpS0;
						fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
					}
					else
					{
						fS0 = -fExtent0;
						fTmpS1 = -(fA01 * fS0 + fB1);
						if (fTmpS1 < -fExtent1)
						{
							fS1 = -fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else if (fTmpS1 <= fExtent1)
						{
							fS1 = fTmpS1;
							fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
						else
						{
							fS1 = fExtent1;
							fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
						}
					}
				}
			}
			else   // region 6 (corner)
			{
				fS1 = -fExtent1;
				fTmpS0 = -(fA01 * fS1 + fB0);
				if (fTmpS0 > fExtent0)
				{
					fS0 = fExtent0;
					fSqrDist = fS0 * (fS0 - 2.0f * fTmpS0) + fS1 * (fS1 + 2.0f * fB1) + fC;
				}
				else if (fTmpS0 >= -fExtent0)
				{
					fS0 = fTmpS0;
					fSqrDist = -fS0 * fS0 + fS1 * (fS1 + 2.0f * fB1) + fC;
				}
				else
				{
					fS0 = -fExtent0;
					fTmpS1 = -(fA01 * fS0 + fB1);
					if (fTmpS1 < -fExtent1)
					{
						fS1 = -fExtent1;
						fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
					else if (fTmpS1 <= fExtent1)
					{
						fS1 = fTmpS1;
						fSqrDist = -fS1 * fS1 + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
					else
					{
						fS1 = fExtent1;
						fSqrDist = fS1 * (fS1 - 2.0f * fTmpS1) + fS0 * (fS0 + 2.0f * fB0) + fC;
					}
				}
			}
		}
	}
	else
	{
		// The segments are parallel.
		coFloat fE0pE1 = fExtent0 + fExtent1;
		coFloat fSign = (fA01 > 0.0f ? -1.0f : 1.0f);
		coFloat b0Avr = 0.5f * (fB0 - fSign * fB1);
		coFloat fLambda = -b0Avr;
		if (fLambda < -fE0pE1)
		{
			fLambda = -fE0pE1;
		}
		else if (fLambda > fE0pE1)
		{
			fLambda = fE0pE1;
		}

		fS1 = -fSign * fLambda * fExtent1 / fE0pE1;
		fS0 = fLambda + fSign * fS1;
		fSqrDist = fLambda * (fLambda + 2.0f * b0Avr) + fC;
	}

	if (pParam0)
	{
		*pParam0 = fS0;
	}
	if (pParam1)
	{
		*pParam1 = fS1;
	}

	// account for numerical round-off error
	return coMax(0.0f, fSqrDist);
}

inline coFloat coSquareDistanceSegmentSegment2(const coVec3& vOrigin0, const coVec3& vExtent0, const coVec3& vOrigin1, const coVec3& vExtent1, coFloat* pParam0 = nullptr, coFloat* pParam1 = nullptr)
{
	//=====
	// Impl from Gu::distanceSegmentSegmentSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentSegment.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// Some conversion is needed between the old & new code
	// Old:
	// segment (s0, s1)
	// origin = s0
	// extent = s1 - s0
	//
	// New:
	// s0 = origin + extent * vDir;
	// s1 = origin - extent * vDir;

	// dsequeira: is this really sensible? We use a highly optimized Wild Magic routine, 
	// then use a segment representation that requires an expensive conversion to/from...

	coVec3 vDir0 = vExtent0;
	const coVec3 vCenter0 = vOrigin0 + vExtent0 * 0.5f;
	coFloat fLength0 = coLength(vExtent0).x;	//AM: change to make it work for degenerate (zero length) segments.
	const coBool b0 = fLength0 != 0.0f;
	coFloat fOneOverLength0 = 0.0f;
	if (b0)
	{
		fOneOverLength0 = 1.0f / fLength0;
		vDir0 *= fOneOverLength0;
		fLength0 *= 0.5f;
	}

	coVec3 vDir1 = vExtent1;
	const coVec3 vCenter1 = vOrigin1 + vExtent1 * 0.5f;
	coFloat fLength1 = coLength(vExtent1).x;
	const coBool b1 = fLength1 != 0.0f;
	coFloat fOneOverLength1 = 0.0f;
	if (b1)
	{
		fOneOverLength1 = 1.0f / fLength1;
		vDir1 *= fOneOverLength1;
		fLength1 *= 0.5f;
	}

	// the return param vals have -extent = s0, extent = s1

	const coFloat fD2 = coSquareDistanceSegmentSegment2(vCenter0, vDir0, fLength0, vCenter1, vDir1, fLength1, pParam0, pParam1);

	//ML : This is wrong for some reason, I guess it has precision issue
	//// renormalize into the 0 = s0, 1 = s1 range
	//if (pParam0)
	//	*pParam0 = b0 ? ((*pParam0) * fOneOverLength0 * 0.5f + 0.5f) : 0.0f;
	//if (pParam1)
	//	*pParam1 = b1 ? ((*pParam1) * fOneOverLength1 * 0.5f + 0.5f) : 0.0f;

	if (pParam0)
	{
		*pParam0 = b0 ? ((fLength0 + (*pParam0)) * fOneOverLength0) : 0.0f;
	}
	if (pParam1)
	{
		*pParam1 = b1 ? ((fLength1 + (*pParam1)) * fOneOverLength1) : 0.0f;
	}

	return fD2;
}

coFORCE_INLINE coVec3 coGetClosestPointPointTriangle2(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& ab, const coVec3& ac)
{
	//=====
	// Impl from closestPtPointTriangle2(), \physx\source\geomutils\src\distance\GuDistancePointTriangle.h
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// Check if P in vertex region outside A
	//const PxVec3 ab = b - a;
	//const PxVec3 ac = c - a;
	const coVec3 ap = p - a;
	const coFloat d1 = coDot(ab, ap).x;
	const coFloat d2 = coDot(ac, ap).x;
	if (d1 <= 0.0f && d2 <= 0.0f)
		return a;	// Barycentric coords 1,0,0

	// Check if P in vertex region outside B
	const coVec3 bp = p - b;
	const coFloat d3 = coDot(ab, bp).x;
	const coFloat d4 = coDot(ac, bp).x;
	if (d3 >= 0.0f && d4 <= d3)
		return b;	// Barycentric coords 0,1,0

	// Check if P in edge region of AB, if so return projection of P onto AB
	const coFloat vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		const coFloat v = d1 / (d1 - d3);
		return a + v * ab;	// barycentric coords (1-v, v, 0)
	}

	// Check if P in vertex region outside C
	const coVec3 cp = p - c;
	const coFloat d5 = coDot(ab, cp).x;
	const coFloat d6 = coDot(ac, cp).x;
	if (d6 >= 0.0f && d5 <= d6)
		return c;	// Barycentric coords 0,0,1

	// Check if P in edge region of AC, if so return projection of P onto AC
	const coFloat vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		const coFloat w = d2 / (d2 - d6);
		return a + w * ac;	// barycentric coords (1-w, 0, w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	const coFloat va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		const coFloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b);	// barycentric coords (0, 1-w, w)
	}

	// P inside face region. Compute Q through its barycentric coords (u,v,w)
	const coFloat denom = 1.0f / (va + vb + vc);
	const coFloat v = vb * denom;
	const coFloat w = vc * denom;
	return a + ab * v + ac * w;
}

inline coVec3 coGetClosestPointPointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, coFloat& s, coFloat& t)
{
	//=====
	// Impl from closestPtPointTriangle(), \PhysX-4.0\physx\source\geomutils\src\contact\GuContactSphereMesh.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// Check if P in vertex region outside A
	const coVec3 ab = b - a;
	const coVec3 ac = c - a;
	const coVec3 ap = p - a;
	const coFloat d1 = coDot(ab, ap).x;
	const coFloat d2 = coDot(ac, ap).x;
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		s = 0.0f;
		t = 0.0f;
		return a;	// Barycentric coords 1,0,0
	}

	// Check if P in vertex region outside B
	const coVec3 bp = p - b;
	const coFloat d3 = coDot(ab, bp).x;
	const coFloat d4 = coDot(ac, bp).x;
	if (d3 >= 0.0f && d4 <= d3)
	{
		s = 1.0f;
		t = 0.0f;
		return b;	// Barycentric coords 0,1,0
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	const coFloat vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		const coFloat v = d1 / (d1 - d3);
		s = v;
		t = 0.0f;
		return a + v * ab;	// barycentric coords (1-v, v, 0)
	}

	// Check if P in vertex region outside C
	const coVec3 cp = p - c;
	const coFloat d5 = coDot(ab, cp).x;
	const coFloat d6 = coDot(ac, cp).x;
	if (d6 >= 0.0f && d5 <= d6)
	{
		s = 0.0f;
		t = 1.0f;
		return c;	// Barycentric coords 0,0,1
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	const coFloat vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		const coFloat w = d2 / (d2 - d6);
		s = 0.0f;
		t = w;
		return a + w * ac;	// barycentric coords (1-w, 0, w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	const coFloat va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		const coFloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		s = 1.0f - w;
		t = w;
		return b + w * (c - b);	// barycentric coords (0, 1-w, w)
	}

	// P inside face region. Compute Q through its barycentric coords (u,v,w)
	const coFloat denom = 1.0f / (va + vb + vc);
	const coFloat v = vb * denom;
	const coFloat w = vc * denom;
	s = v;
	t = w;
	return a + ab * v + ac * w;
}

coFORCE_INLINE coFloat coSquareDistancePointTriangle(const coVec3& vPoint, const coVec3& vTriangleOrigin, const coVec3& vTriangleEdge0, const coVec3& vTriangleEdge1,
	coFloat* pParam0 = nullptr, coFloat* pParam1 = nullptr)
{
	//=====
	// Impl from Gu::distancePointTriangleSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistancePointTriangle.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	const coVec3 vPt0 = vTriangleEdge0 + vTriangleOrigin;
	const coVec3 vPt1 = vTriangleEdge1 + vTriangleOrigin;
	coFloat fS, fT;
	const coVec3 vCP = coGetClosestPointPointTriangle(vPoint, vTriangleOrigin, vPt0, vPt1, fS, fT);
	if (pParam0)
		*pParam0 = fS;
	if (pParam1)
		*pParam1 = fT;
	return coSquareLength(vCP - vPoint).x;
}

inline coFloat coSquareDistanceSegmentTriangle(const coVec3& vOrigin, const coVec3& vSegment, const coVec3& vP0, const coVec3& vTriEdge0, const coVec3& vTriEdge1)
{
	//=====
	// Impl from Gu::distanceSegmentTriangleSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
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
	const coVec3 vNormal = coCross(vTriEdge0, vTriEdge1);
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
