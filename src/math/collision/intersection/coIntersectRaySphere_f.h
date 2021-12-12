// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3_f.h"

// Based on GD Mag code, but now works correctly when origin is inside the sphere.
// This version has limited accuracy.
inline coBool coIntersectRaySphereBasic(const coVec3& origin, const coVec3& dir, coFloat length, const coVec3& center, coFloat radius, coFloat& dist, coVec3* hit_pos = nullptr)
{
	//=====
	// Impl from Gu::intersectRaySphereBasic(), \physx\source\geomutils\src\intersection\GuIntersectionRaySphere.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// get the offset vector
	const coVec3 offset = center - origin;

	// get the distance along the ray to the center point of the sphere
	const coFloat ray_dist = coDot(dir, offset).x;

	// get the squared distances
	const coFloat off2 = coDot(offset, offset).x;
	const coFloat rad_2 = radius * radius;
	if (off2 <= rad_2)
	{
		// we're in the sphere
		if (hit_pos)
			*hit_pos = origin;
		dist = 0.0f;
		return true;
	}

	if (ray_dist <= 0 || (ray_dist - length) > radius)
	{
		// moving away from object or too far away
		return false;
	}

	// find hit distance squared
	const coFloat d = rad_2 - (off2 - ray_dist * ray_dist);
	if (d < 0.0f)
	{
		// ray passes by sphere without hitting
		return false;
	}

	// get the distance along the ray
	dist = ray_dist - coSquareRoot(d);
	if (dist > length)
	{
		// hit point beyond length
		return false;
	}

	// sort out the details
	if (hit_pos)
		*hit_pos = origin + dir * dist;
	return true;
}

// PT: modified version calls the previous function, but moves the ray origin closer to the sphere. The test accuracy is
// greatly improved as a result. This is an idea proposed on the GD-Algorithms list by Eddie Edwards.
// See: http://www.codercorner.com/blog/?p=321
inline coBool coIntersectRaySphere(const coVec3& origin, const coVec3& dir, coFloat length, const coVec3& center, coFloat radius, coFloat& dist, coVec3* hit_pos = nullptr)
{
	//=====
	// Impl from Gu::intersectRaySphere(), \physx\source\geomutils\src\intersection\GuIntersectionRaySphere.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// PT: small distance between a ray origin and a potentially hit surface. Should be small enough to
	// limit accuracy issues coming from large distance values, but not too close to the surface to make
	// sure we don't start inside the shape.
	constexpr coFloat raySurfaceOffset = 10.0f;

	const coVec3 x = origin - center;
	coFloat l = coSquareRoot(coDot(x, x)).x - radius - raySurfaceOffset;

	//	if(l<0.0f)
	//		l=0.0f;
	l = coMax(l, 0.0f);

	coBool status = coIntersectRaySphereBasic(origin + l * dir, dir, length - l, center, radius, dist, hit_pos);
	if (status)
	{
		//		dist += l/length;
		dist += l;
	}
	return status;
}
