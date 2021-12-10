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
