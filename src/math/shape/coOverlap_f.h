// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coDistance_f.h"
#include <math/shape/coAabb_f.h>
#include <math/shape/coSphere.h>
#include <math/shape/coPlane.h>
#include <math/shape/coSegment_f.h>
#include <math/shape/coRay.h>
#include <math/vector/coFloatx3_f.h>
#include <math/vector/coFloatx4_f.h>
#include <math/vector/coBool32x3_f.h>

// TODO: Should be moved to the math/collision directory

coFORCE_INLINE coBool32x3 coOverlapSolid(const coAabb& aabb, const coVec3& point) { return point >= aabb.min & point <= aabb.max; }
coFORCE_INLINE coBool coOverlapSolid(const coAabb& aabb, const coSegment& seg)
{
	// Refs:
	// - Another View on the Classic Ray-AABB Intersection Algorithm for BVH Traversal, Roman Wiche
	// https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
	// - Fast, Branchless Ray/Bounding Box Intersections, Tavian Barnes
	// https://tavianator.com/2011/ray_box.html
	// https://tavianator.com/2015/ray_box_nan.html
	// Notes: For any algo, it's important that it manages axis-aligned directions which causes invDir to be inf.

	const coVec3 delta = seg.p1 - seg.p0;
	const coFloatx4 len = coLength(delta);
	const coVec3 invDir = coInvert(delta / len.x);
	const coVec3 t1 = (aabb.min - seg.p0) * invDir;
	const coVec3 t2 = (aabb.max - seg.p0) * invDir;
	const coVec3 tmin1 = coMin(t1, t2);
	const coVec3 tmax1 = coMax(t1, t2);
	const coFloat tmin = coMax(0.0f, coMax(tmin1).x);
	const coFloat tmax = coMin(len.x, coMin(tmax1).x);
	return tmin <= tmax;
}

coFORCE_INLINE coBool coOverlapSolid(const coAabb& aabb, const coVec3& vPos, const coVec3& invDir)
{
	// - Another View on the Classic Ray-AABB Intersection Algorithm for BVH Traversal, Roman Wiche
	// https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
	// Notes: For any algo, it's important that it manages axis-aligned directions which causes invDir to be inf.
	const coVec3 t1 = (aabb.min - vPos) * invDir;
	const coVec3 t2 = (aabb.max - vPos) * invDir;
	const coVec3 tmin1 = coMin(t1, t2);
	const coVec3 tmax1 = coMax(t1, t2);
	const coFloatx3 tmin = coMax(0.0f, coMax(tmin1).x);
	const coFloatx3 tmax = coMin(tmax1);
	return coAreAllTrue(tmin <= tmax);
}

coFORCE_INLINE coBool32x4 coOverlapSolid(const coAabb& aabb, const coPlane& plane)
{
	const coVec3 center = coGetCenter(aabb);
	const coVec3 halfExtents = aabb.max - center;
	const coVec3 normal = coVec3(plane.normalAndDistance);
	const coFloatx4 r = coDot(halfExtents, coAbs(normal));
	const coFloatx4 dist = coDot(plane.normalAndDistance, coVec4(center, 1.0f));
	return coAbs(dist) <= r;
}
coBool coOverlapXY(const coSegment& _a, const coSegment& _b, coVec3& _hit);
coFORCE_INLINE coBool32x3 coOverlapSolidSolid(const coAabb& a, const coAabb& b) { return coNot(coIsEmpty(coIntersect(a, b))); }

//coFORCE_INLINE coBool coOverlapSolidHollow(const coAabb& aabb, )