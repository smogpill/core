// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb_f.h>
#include <math/shape/coSphere.h>
#include <math/vector/coFloatx3_f.h>
#include <math/vector/coBool32x3.h>

coFORCE_INLINE coBool32x3 coOverlapSolid(const coAabb& aabb, const coVec3& point) { return point >= aabb.min && point <= aabb.max; }
coFORCE_INLINE coBool32x3 coOverlapSolidSolid(const coAabb& a, const coAabb& b) { return coIsEmpty(coIntersect(a, b)); }
coFORCE_INLINE coBool coOverlapSolidHollow(const coAabb& aabb, const coSphere& s)
{
	// https://stackoverflow.com/questions/28343716/sphere-intersection-test-of-aabb
	coFloat dmax = 0.0f;
	coFloat dmin = 0.0f;
	for (coUint i = 0; i < 3; ++i)
	{
		const coFloat a = coPow2(s.centerAndRadius[i] - aabb.min[i]);
		const coFloat b = coPow2(s.centerAndRadius[i] - aabb.max[i]);
		dmax += coMax(a, b);
		if (s.centerAndRadius[i] < aabb.min[i])
			dmin += a;
		else if (s.centerAndRadius[i] > aabb.max[i])
			dmin += b;
	}
	const coFloat radius2 = s.centerAndRadius.w * s.centerAndRadius.w;
	return dmin <= radius2 && radius2 <= dmax;
}

//coFORCE_INLINE coBool coOverlapSolidHollow(const coAabb& aabb, )