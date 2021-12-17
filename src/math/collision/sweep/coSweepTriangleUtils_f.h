// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>
#include <math/vector/coVec3_f.h>
#include "math/shape/coDistance_f.h"
#include "math/shape/triangle/coTriangle_f.h"
#include "math/collision/distance/coDistancePointTriangle_f.h"
#include "../query/coQueries.h"

constexpr coFloat GU_EPSILON_SAME_DISTANCE = 1e-3f;

coFORCE_INLINE coUint32 coGetInitIndex(const coUint32* coRESTRICT cachedIndex, coUint32 nbTris)
{
	coUint32 initIndex = 0;	// PT: by default the first triangle to process is just the first one in the array
	if (cachedIndex)			// PT: but if we cached the last closest triangle from a previous call...
	{
		coASSERT(*cachedIndex < nbTris);
		initIndex = *cachedIndex;	// PT: ...then we should start with that one, to potentially shrink the ray as early as possible
	}
	return initIndex;
}

coFORCE_INLINE coBool coShouldFlipNormal(const coVec3& normal, coBool meshBothSides, coBool isDoubleSided, const coVec3& triangleNormal, const coVec3& dir)
{
	// PT: this function assumes that input normal is opposed to the ray/sweep direction. This is always
	// what we want except when we hit a single-sided back face with 'meshBothSides' enabled.

	if (!meshBothSides || isDoubleSided)
		return false;

	coASSERT(coDot(normal, dir).x <= 0.0f);	// PT: if this fails, the logic below cannot be applied
	return coDot(triangleNormal, dir).x > 0.0f;	// PT: true for back-facing hits
}

coFORCE_INLINE coUint32 coGetTriangleIndex(coUint32 i, coUint32 cachedIndex)
{
	coUint32 triangleIndex;
	if (i == 0)
		triangleIndex = cachedIndex;
	else if (i == cachedIndex)
		triangleIndex = 0;
	else
		triangleIndex = i;
	return triangleIndex;
}

// PT: quick triangle culling for sphere-based sweeps
// Please refer to %SDKRoot%\InternalDocumentation\GU\coarseCulling.png for details & diagram.
coFORCE_INLINE coBool coCoarseCullingTri(const coVec3& center, const coVec3& dir, coFloat t, coFloat radius, const coVec3* coRESTRICT triVerts)
{
	// PT: compute center of triangle ### could be precomputed?
	const coVec3 triCenter = (triVerts[0] + triVerts[1] + triVerts[2]) * (1.0f / 3.0f);

	// PT: distance between the triangle center and the swept path (an LSS)
	// Same as: distancePointSegmentSquared(center, center+dir*t, TriCenter);
	coFloat d = coSquareRoot(coSquareDistancePointSegment3(center, dir, t, triCenter)) - radius - 0.0001f;

	if (d < 0.0f)	// The triangle center lies inside the swept sphere
		return true;

	d *= d;

	// PT: coarse capsule-vs-triangle overlap test ### distances could be precomputed?
	if (1)
	{
		if (d <= coSquareLength(triCenter - triVerts[0]).x)
			return true;
		if (d <= coSquareLength(triCenter - triVerts[1]).x)
			return true;
		if (d <= coSquareLength(triCenter - triVerts[2]).x)
			return true;
	}
	else
	{
		const coFloat d0 = coSquareLength(triCenter - triVerts[0]).x;
		const coFloat d1 = coSquareLength(triCenter - triVerts[1]).x;
		const coFloat d2 = coSquareLength(triCenter - triVerts[2]).x;
		coFloat triRadius = coMax(d0, d1);
		triRadius = coMax(triRadius, d2);
		if (d <= triRadius)
			return true;
	}
	return false;
}

// PT: quick triangle rejection for sphere-based sweeps.
// Please refer to %SDKRoot%\InternalDocumentation\GU\cullTriangle.png for details & diagram.
coFORCE_INLINE coBool coCullTriangle(const coVec3* coRESTRICT triVerts, const coVec3& dir, coFloat radius, coFloat t, const coFloat dpc0)
{
	// PT: project triangle on axis
	const coFloat dp0 = coDot(triVerts[0], dir).x;
	const coFloat dp1 = coDot(triVerts[1], dir).x;
	const coFloat dp2 = coDot(triVerts[2], dir).x;

	// PT: keep min value = earliest possible impact distance
	coFloat dp = dp0;
	dp = coMin(dp, dp1);
	dp = coMin(dp, dp2);

	// PT: make sure we keep triangles that are about as close as best current distance
	radius += 0.001f + GU_EPSILON_SAME_DISTANCE;

	// PT: if earliest possible impact distance for this triangle is already larger than
	// sphere's current best known impact distance, we can skip the triangle
	if (dp > dpc0 + t + radius)
	{
		//PX_ASSERT(resx == 0.0f);
		return false;
	}

	// PT: if triangle is fully located before the sphere's initial position, skip it too
	const coFloat dpc1 = dpc0 - radius;
	if (dp0 < dpc1 && dp1 < dpc1 && dp2 < dpc1)
	{
		//PX_ASSERT(resx == 0.0f);
		return false;
	}

	//PX_ASSERT(resx != 0.0f);
	return true;
}

// PT: combined triangle culling for sphere-based sweeps
coFORCE_INLINE coBool coRejectTriangle(const coVec3& center, const coVec3& unitDir, coFloat curT, coFloat radius, const coVec3* coRESTRICT triVerts, const coFloat dpc0)
{
	if (!coCoarseCullingTri(center, unitDir, curT, radius, triVerts))
		return true;
	if (!coCullTriangle(triVerts, unitDir, radius, curT, dpc0))
		return true;
	return false;
}

inline void coComputeSphereTriImpactData(coVec3& hit, coVec3& normal, const coVec3& center, const coVec3& dir, coFloat t, const coTriangle& tri)
{
	const coVec3 newSphereCenter = center + dir * t;

	// We need the impact point, not computed by the new code
	coFloat u_unused, v_unused;
	const coVec3 localHit = coClosestPtPointTriangle(newSphereCenter, tri.a, tri.b, tri.c, u_unused, v_unused);

	// This is responsible for the cap-vs-box stuck while jumping. However it's needed to slide on box corners!
	// PT: this one is also dubious since the sphere/capsule center can be far away from the hit point when the radius is big!
	coVec3 localNormal = newSphereCenter - localHit;
	const coFloat m = coLength(localNormal).x;
	if (m > 0.0f)
		localNormal /= m;
	if (m < 1e-3f)
		localNormal = coGetNormal(tri);

	hit = localHit;
	normal = localNormal;
}

// PT: implements the spec for IO sweeps in a single place (to ensure consistency)
coFORCE_INLINE coBool coSetInitialOverlapResults(coSweepHit& hit, const coVec3& unitDir, coUint32 faceIndex)
{
	// PT: please write these fields in the order they are listed in the struct.
	hit.faceIndex = faceIndex;
	hit.flags = coHitFlag::eNORMAL | coHitFlag::eFACE_INDEX;
	hit.normal = -unitDir;
	hit.distance = 0.0f;
	return true;	// PT: true indicates a hit, saves some lines in calling code
}
