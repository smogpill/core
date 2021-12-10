// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3_f.h"
#include "math/shape/coSegment_f.h"
#include "math/shape/coPlane_f.h"
#include "math/shape/triangle/coTriangle.h"

coFORCE_INLINE coFloat coSigned2DTriArea(const coVec3& a, const coVec3& b, const coVec3& c)
{
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

inline coBool coIntersectSegmentSegment(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& d, coFloat& t)
{
	// 5.1.9.1 2D Segment Intersection, Realtime Collision Detection book, Christer Ericson
	const coFloat a1 = coSigned2DTriArea(a, b, d);
	const coFloat a2 = coSigned2DTriArea(a, b, c);
	if (a1 * a2 < 0.0f)
	{
		const coFloat a3 = coSigned2DTriArea(c, d, a);
		const coFloat a4 = a3 + a2 - a1;
		if (a3 * a4 < 0.0f)
		{
			t = a3 / (a3 - a4);
			return true;
		}
	}

	return false;
}

coFORCE_INLINE coBool coIntersect(const coPlane& plane, const coSegment& seg, coVec3& p)
{
	const coVec3 delta = seg.p1 - seg.p0;
	// Ok even when denominator is zero, no need for tests
	const coFloatx4 t = (-coBroadcastW(plane.normalAndDistance) - coDot(coVec3(plane.normalAndDistance), seg.p0)) / coDot(coVec3(plane.normalAndDistance), delta);
	if (t >= 0.0f && t <= 1.0f)
	{
		p = seg.p0 + t * delta;
		return true;
	}
	return false;
}

coFORCE_INLINE coBool coIntersect(const coRay& ray, const coTriangle& triangle, coFloat& t, coFloat& u, coFloat& v, coBool culling = false, coFloat epsilon = 1e-7f)
{
	// Möller-Trumbore algorithm
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

	const coVec3 v0v1 = triangle.b - triangle.a;
	const coVec3 v0v2 = triangle.c - triangle.a;
	const coVec3 pvec = coCross(ray.GetDir(), v0v2);
	const coFloatx4 det = coDot(v0v1, pvec);
	if (culling)
	{
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < epsilon) return false;
	}
	else
	{
		// ray and triangle are parallel if det is close to 0
		if (coAbs(det) < epsilon) return false;
	}

	const coFloatx4 invDet = 1.0f / det;

	const coVec3 tvec = ray.origin - triangle.a;
	u = (coDot(tvec, pvec) * invDet).x;
	if (u < 0 || u > 1) return false;

	coVec3 qvec = coCross(tvec, v0v1);
	v = (coDot(ray.GetDir(), qvec) * invDet).x;
	if (v < 0 || u + v > 1) return false;

	t = (coDot(v0v2, qvec) * invDet).x;
	return t < ray.dirAndLength.w;
}

coFORCE_INLINE coBool coIntersect(const coRay& ray, const coTriangle& triangle, coBool culling = false, coFloat epsilon = 1e-7f)
{
	coFloat t, u, v;
	return coIntersect(ray, triangle, t, u, v, culling, epsilon);
}

coFORCE_INLINE coBool coIntersect(const coRay& ray, const coTriangle& triangle, coFloat& t, coBool culling = false, coFloat epsilon = 1e-7f)
{
	coFloat u, v;
	return coIntersect(ray, triangle, t, u, v, culling, epsilon);
}

coFORCE_INLINE coUint32 _coGetClosestAxis(const coVec3& vV, coUint32& uiJ, coUint32& uiK)
{
	//=====
	// Impl from closestAxis(), \PhysX-4.0\physx\source\foundation\include\PsMathUtils.h
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// find largest 2D plane projection
	const coFloat fAbsPx = coAbs(vV.x);
	const coFloat fAbsNy = coAbs(vV.y);
	const coFloat fAbsNz = coAbs(vV.z);

	coUint32 uiM = 0; // x biggest axis
	uiJ = 1;
	uiK = 2;
	if (fAbsNy > fAbsPx && fAbsNy > fAbsNz)
	{
		// y biggest
		uiJ = 2;
		uiK = 0;
		uiM = 1;
	}
	else if (fAbsNz > fAbsPx)
	{
		// z biggest
		uiJ = 0;
		uiK = 1;
		uiM = 2;
	}
	return uiM;
}

coFORCE_INLINE coBool coIntersectEdgeEdge(const coVec3& vP1, const coVec3& vP2, const coVec3& vDir, const coVec3& vP3, const coVec3& vP4, coFloat& fDist, coVec3& vImpactPoint)
{
	//=====
	// Impl from Gu::intersectEdgeEdge(), \PhysX-4.0\physx\source\geomutils\src\intersection\GuIntersectionEdgeEdge.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	auto GetSignedDistanceFromPlane = [](const coVec4& vPlane, const coVec3& vPoint)
	{
		return coDot(vPlane, coVec4(vPoint, 1.0f));
	};

	const coVec3 v1 = vP2 - vP1;

	// Build plane P based on edge (vP1, vP2) and direction (vDir)
	const coVec3 vPlaneNormal = coCross(v1, vDir);
	const coVec4 vPlane = coVec4(vPlaneNormal, -coDot(vPlaneNormal, vP1));

	// if colliding edge (vP3,vP4) does not cross plane return no collision
	// same as if vP3 and vP4 on same side of plane return 0
	//
	// Derivation:
	// fD3 = d(vP3, P) = (vP3 | plane.n) - plane.d;		Reversed sign compared to Plane::Distance() because plane.d is negated.
	// d4 = d(vP4, P) = (vP4 | plane.n) - plane.d;		Reversed sign compared to Plane::Distance() because plane.d is negated.
	// if fD3 and d4 have the same sign, they're on the same side of the plane => no collision
	// We test both sides at the same time by only testing Sign(fD3 * d4).
	// ### put that in the Plane class
	// ### also check that code in the triangle class that might be similar
	const coFloat fD3 = GetSignedDistanceFromPlane(vPlane, vP3).x;
	coFloat fTemp = fD3 * GetSignedDistanceFromPlane(vPlane, vP4).x;
	if (fTemp > 0.0f)
	{
		return false;
	}

	// if colliding edge (vP3,vP4) and plane are parallel return no collision
	coVec3 v2 = vP4 - vP3;

	fTemp = coDot(vPlaneNormal, v2).x;
	if (fTemp == 0.0f)
	{
		return false;	// ### epsilon would be better
	}

	// compute intersection point of plane and colliding edge (vP3,vP4)
	vImpactPoint = vP3 - v2 * (fD3 / fTemp);

	// find largest 2D plane projection
	coUint32 uiI, uiJ;
	_coGetClosestAxis(vPlaneNormal, uiI, uiJ);

	// compute distance of intersection from line (vImpactPoint, -vDir) to line (vP1,vP2)
	fDist = (v1[uiI] * (vImpactPoint[uiJ] - vP1[uiJ]) - v1[uiJ] * (vImpactPoint[uiI] - vP1[uiI])) / (v1[uiI] * vDir[uiJ] - v1[uiJ] * vDir[uiI]);
	if (fDist < 0.0f)
	{
		return false;
	}

	// compute intersection point on edge (vP1,vP2) line
	vImpactPoint -= fDist * vDir;

	// check if intersection point (vImpactPoint) is between edge (vP1,vP2) vertices
	fTemp = (vP1.x - vImpactPoint.x) * (vP2.x - vImpactPoint.x) + (vP1.y - vImpactPoint.y) * (vP2.y - vImpactPoint.y) + (vP1.z - vImpactPoint.z) * (vP2.z - vImpactPoint.z);
	if (fTemp < 1e-3f)
	{
		return true;	// collision found
	}

	return false;	// no collision
}

coFORCE_INLINE coBool coIntersectRayTriangle(const coVec3& vOrigin, const coVec3& vDir,	const coVec3& vA, const coVec3& vB, const coVec3& vC,
	coFloat& fOutT, coFloat& fOutU, coFloat& fOutV, coFloat fEnlarge)
{
	//=====
	// Impl from intersectRayTriangle(), \PhysX-4.0\physx\source\geomutils\src\intersection\GuIntersectionRayTriangle.h
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// Find vectors for two edges sharing vA
	const coVec3 vAB = vB - vA;
	const coVec3 vAC = vC - vA;

	// Begin calculating determinant - also used to calculate U parameter
	const coVec3 vPVec = coCross(vDir, vAC); // error ~ |v2-v0|

														// If determinant is near zero, ray lies in plane of triangle
	const coFloat fDet = coDot(vAB, vPVec).x; // error ~ |v2-v0|*|v1-v0|

	if (fDet < FLT_EPSILON * FLT_EPSILON)
		return false;

	// Calculate distance from vert0 to ray origin
	const coVec3 vTVec = vOrigin - vA;

	// Calculate U parameter and test bounds
	const coFloat fU = coDot(vTVec, vPVec).x;

	const coFloat fEnlargeCoeff = fEnlarge * fDet;
	const coFloat fUVLimit = -fEnlargeCoeff;
	const coFloat fUVLimit2 = fDet + fEnlargeCoeff;

	if (fU<fUVLimit || fU>fUVLimit2)
		return false;

	// Prepare to test V parameter
	const coVec3 vQVec = coCross(vTVec, vAB);

	// Calculate V parameter and test bounds
	const coFloat fV = coDot(vDir, vQVec).x;
	if (fV<fUVLimit || (fU + fV)>fUVLimit2)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	const coFloat fT = coDot(vAC, vQVec).x;

	const coFloat fInvDet = 1.0f / fDet;
	fOutT = fT * fInvDet;
	fOutU = fU * fInvDet;
	fOutV = fV * fInvDet;
	return true;
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

inline coFloat coGetDistanceSegmentSegmentSquared(const coVec3& vOrigin0, const coVec3& vDir0, coFloat fExtent0,
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

inline coFloat coGetDistanceSegmentSegmentSquared(const coVec3& vOrigin0, const coVec3& vExtent0, const coVec3& vOrigin1, const coVec3& vExtent1, coFloat* pParam0 = nullptr, coFloat* pParam1 = nullptr)
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

	const coFloat fD2 = coGetDistanceSegmentSegmentSquared(vCenter0, vDir0, fLength0, vCenter1, vDir1, fLength1, pParam0, pParam1);

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

coFORCE_INLINE coFloat coGetDistancePointTriangleSquared(const coVec3& vPoint, const coVec3& vTriangleOrigin, const coVec3& vTriangleEdge0, const coVec3& vTriangleEdge1,
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

inline coFloat coGetDistanceSegmentTriangleSquared(const coVec3& vOrigin, const coVec3& vSegment, const coVec3& vP0, const coVec3& vTriEdge0, const coVec3& vTriEdge1)
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
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3m
					{
						// minimum on face s=0 or r=0
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					fSqrDist0 = coGetDistancePointTriangleSquared(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5m
				{
					// minimum on face t=0 or r=0
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					fSqrDist0 = coGetDistancePointTriangleSquared(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0m
				{
					// minimum on face r=0
					fSqrDist = coGetDistancePointTriangleSquared(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 0.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2m
				{
					// minimum on face s=0 or s+t=1 or r=0
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6m
				{
					// minimum on face t=0 or s+t=1 or r=0
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1m
				{
					// minimum on face s+t=1 or r=0
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				fSqrDist0 = coGetDistancePointTriangleSquared(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
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
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3
					{
						// minimum on face s=0
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
				}
				else if (fT < 0.0f)  // region 5
				{
					// minimum on face t=0
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
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
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6
				{
					// minimum on face t=0 or s+t=1
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1
				{
					// minimum on face s+t=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
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
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3p
					{
						// minimum on face s=0 or r=1
						fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coGetDistancePointTriangleSquared(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5p
				{
					// minimum on face t=0 or r=1
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;

					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coGetDistancePointTriangleSquared(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0p
				{
					// minimum face on r=1
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist = coGetDistancePointTriangleSquared(kPt, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 1.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2p
				{
					// minimum on face s=0 or s+t=1 or r=1
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6p
				{
					// minimum on face t=0 or s+t=1 or r=1
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1p
				{
					// minimum on face s+t=1 or r=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				const coVec3 kPt = vOrigin + vSegment;
				fSqrDist0 = coGetDistancePointTriangleSquared(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
				fR0 = 1.0f;
				_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
			}
		}
	}
	else
	{
		// Segment and triangle are parallel
		fSqrDist = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
		fT = 0.0f;

		fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, vP0, vTriEdge1, &fR0, &fT0);
		fS0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kTriSegOrig = vP0 + vTriEdge0;
		const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
		fSqrDist0 = coGetDistanceSegmentSegmentSquared(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
		fS0 = 1.0f - fT0;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		fSqrDist0 = coGetDistancePointTriangleSquared(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kPt = vOrigin + vSegment;
		fSqrDist0 = coGetDistancePointTriangleSquared(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 1.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
	}

	// Account for numerical round-off error
	return coMax(0.0f, fSqrDist);
}
