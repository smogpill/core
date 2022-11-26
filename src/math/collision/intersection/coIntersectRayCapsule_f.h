// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3_f.h"
#include "math/shape/capsule/coCapsule.h"
#include "math/shape/coDistance_f.h"

coUint32 _coIntersectRayCapsuleInternal(const coVec3& origin, const coVec3& dir, const coVec3& p0, const coVec3& p1, coFloat radius, coFloat s[2]);

coFORCE_INLINE coBool coIntersectRayCapsule(const coVec3& origin, const coVec3& dir, const coVec3& p0, const coVec3& p1, coFloat radius, coFloat& t)
{
	//=====
	// Impl from Gu::intersectRayCapsule(), \physx\source\geomutils\src\intersection\GuIntersectionRayCapsule.h
	// Most comments are from the original code.
	// From the open source version of the PhysX SDK
	//=====

	// PT: move ray origin close to capsule, to solve accuracy issues.
	// We compute the distance D between the ray origin and the capsule's segment.
	// Then E = D - radius = distance between the ray origin and the capsule.
	// We can move the origin freely along 'dir' up to E units before touching the capsule.
	coFloat l = coSquareDistancePointSegment2(p0, p1 - p0, origin);
	l = coSquareRoot(l) - radius;

	// PT: if this becomes negative or null, the ray starts inside the capsule and we can early exit
	if (l <= 0.0f)
	{
		t = 0.0f;
		return true;
	}

	// PT: small distance between a ray origin and a potentially hit surface. Should be small enough to
	// limit accuracy issues coming from large distance values, but not too close to the surface to make
	// sure we don't start inside the shape.
	constexpr coFloat raySurfaceOffset = 10.0f;

	// PT: we remove an arbitrary GU_RAY_SURFACE_OFFSET units to E, to make sure we don't go close to the surface.
	// If we're moving in the direction of the capsule, the origin is now about GU_RAY_SURFACE_OFFSET units from it.
	// If we're moving away from the capsule, the ray won't hit the capsule anyway.
	// If l is smaller than GU_RAY_SURFACE_OFFSET we're close enough, accuracy is good, there is nothing to do.
	if (l > raySurfaceOffset)
		l -= raySurfaceOffset;
	else
		l = 0.0f;

	// PT: move origin closer to capsule and do the raycast
	coFloat s[2];
	const coUint32 nbHits = _coIntersectRayCapsuleInternal(origin + l * dir, dir, p0, p1, radius, s);
	if (!nbHits)
		return false;

	// PT: keep closest hit only
	if (nbHits == 1)
		t = s[0];
	else
		t = (s[0] < s[1]) ? s[0] : s[1];

	// PT: fix distance (smaller than expected after moving ray close to capsule)
	t += l;
	return true;
}

coFORCE_INLINE coBool coIntersectRayCapsule(const coVec3& origin, const coVec3& dir, const coCapsule& capsule, coFloat& t)
{
	return coIntersectRayCapsule(origin, dir, capsule.a, capsule.b, capsule.radius, t);
}