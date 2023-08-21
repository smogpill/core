// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../shape/coAabb_f.h"
#include "../../../shape/aabb/coAABox4_f.h"
#include "../../../shape/ray/coRayx4.h"
#include "../../../vector/coVec3x4_f.h"

coFORCE_INLINE coBool32x4 coOverlapSolidSolid(const coAABox4& a, const coRay2x4& ray)
{
	// Refs:
	// - Another View on the Classic Ray-AABB Intersection Algorithm for BVH Traversal, Roman Wiche
	// https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
	// - Fast, Branchless Ray/Bounding Box Intersections, Tavian Barnes
	// https://tavianator.com/2011/ray_box.html
	// https://tavianator.com/2015/ray_box_nan.html

	const coVec3x4 t1 = (a._min - ray._origin) * ray._invDir;
	const coVec3x4 t2 = (a._max - ray._origin) * ray._invDir;
	const coVec3x4 tmin1 = coMin(t1, t2);
	const coVec3x4 tmax1 = coMax(t1, t2);
	const coFloatx4 tmin = coMax(0.0f, coMax(tmin1));
	const coFloatx4 tmax = coMin(ray._length, coMin(tmax1));
	return tmin <= tmax;

	// Inigo's solution might not work when the ray starts inside the cube (not sure though, should be analyzed).
	// https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
	// 		const VECTOR4 n = invDir*ray.GetOrigin();
	// 		const VECTOR4 k = VECTOR4::Abs(invDir)*a.GetSizeVector() * 0.5f;
	// 		const VECTOR4 t1 = -n - k;
	// 		const VECTOR4 t2 = -n + k;
	// 		const Float32 tN = t1.GetMaxValue3();
	// 		const Float32 tF = t2.GetMinValue3();
	// 		return tN <= tF && tF >= 0.0f;
}
