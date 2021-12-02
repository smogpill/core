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
