// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/coCppExtensions.h>
#include <math/vector/coVec3.h>
#include "coSweepTriangleUtils_f.h"
#include "../query/coQueries.h"
class coVec3;
class coTriangle;

coBool coSweepSphereTriangle(const coVec3* coRESTRICT triVerts, const coVec3& normal, const coVec3& center, coFloat radius, const coVec3& dir, coFloat& impactDistance, coBool& directHit, coBool testInitialOverlap);
coBool coSweepSphereTriangles(coUint32 nbTris, const coTriangle* coRESTRICT triangles, const coVec3& center, const coFloat radius, const coVec3& unitDir, coFloat distance,											// Ray data
	const coUint32* coRESTRICT cachedIndex, coSweepHit& h, coVec3& triNormalOut, coBool isDoubleSided, coBool meshBothSides, coBool anyHit, coBool testInitialOverlap);
void coComputeSphereTriImpactData(coVec3& hit, coVec3& normal, const coVec3& center, const coVec3& dir, float t, const coTriangle& tri);

// PT: computes proper impact data for sphere-sweep-vs-tri, after the closest tri has been found
coFORCE_INLINE coBool coComputeSphereTriangleImpactData(coSweepHit& h, coVec3& triNormalOut, coUint32 index, coFloat curT,
	const coVec3& center, const coVec3& unitDir, const coVec3& bestTriNormal,
	const coTriangle* coRESTRICT triangles,
	coBool isDoubleSided, coBool meshBothSides)
{
	if (index == coUint32(-1))
		return false;	// We didn't touch any triangle

	// Compute impact data only once, using best triangle
	coVec3 hitPos, normal;
	coComputeSphereTriImpactData(hitPos, normal, center, unitDir, curT, triangles[index]);

	// PT: by design, returned normal is opposed to the sweep direction.
	if (coShouldFlipNormal(normal, meshBothSides, isDoubleSided, bestTriNormal, unitDir))
		normal = -normal;

	h.position = hitPos;
	h.normal = normal;
	h.distance = curT;
	h.faceIndex = index;
	h.flags = coHitFlag::eNORMAL | coHitFlag::ePOSITION;
	triNormalOut = bestTriNormal;
	return true;
}