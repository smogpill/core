// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSweepCapsuleTriangle_f.h"
#include "coSweepSphereTriangle_f.h"
#include "math/shape/capsule/coCapsule.h"
#include "math/vector/coVec3_f.h"
#include "math/collision/intersection/coIntersectCapsuleTriangle_f.h"

//=====
// Impl from \physx\source\geomutils\src\sweep\GuSweepCapsuleTriangle.cpp from PhysX 4
// Most comments are from the original code.
// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
//=====

#define COLINEARITY_EPSILON 0.00001f

///////////////////////////////////////////////////////////////////////////////

#define OUTPUT_TRI(pp0, pp1, pp2){															\
	extrudedTris[nbExtrudedTris].verts[0] = pp0;											\
	extrudedTris[nbExtrudedTris].verts[1] = pp1;											\
	extrudedTris[nbExtrudedTris].verts[2] = pp2;											\
	extrudedTris[nbExtrudedTris].denormalizedNormal(extrudedTrisNormals[nbExtrudedTris]);	\
	nbExtrudedTris++;}

#define OUTPUT_TRI2(p0, p1, p2, d){ \
	coTriangle& tri = extrudedTris[nbExtrudedTris]; \
	tri.verts[0] = p0; \
	tri.verts[1] = p1; \
	tri.verts[2] = p2; \
	coVec3 nrm; \
	tri.denormalizedNormal(nrm); \
	if(coDot(nrm, d).x>0.0f) { \
	coVec3 tmp = tri.verts[1]; \
	tri.verts[1] = tri.verts[2]; \
	tri.verts[2] = tmp; \
	nrm = -nrm; \
	} \
	extrudedTrisNormals[nbExtrudedTris] = nrm; \
	nbExtrudedTris++; }

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

// PT: combined triangle culling for sphere-based sweeps
coFORCE_INLINE coBool coRejectTriangle(const coVec3& center, const coVec3& unitDir, coFloat curT, coFloat radius, const coVec3* coRESTRICT triVerts, const coFloat dpc0)
{
	if (!coCoarseCullingTri(center, unitDir, curT, radius, triVerts))
		return true;
	if (!coCullTriangle(triVerts, unitDir, radius, curT, dpc0))
		return true;
	return false;
}

// PT: implements the spec for IO sweeps in a single place (to ensure consistency)
coFORCE_INLINE coBool setInitialOverlapResults(PxSweepHit& hit, const coVec3& unitDir, coUint32 faceIndex)
{
	// PT: please write these fields in the order they are listed in the struct.
	hit.faceIndex = faceIndex;
	hit.flags = PxHitFlag::eNORMAL | PxHitFlag::eFACE_INDEX;
	hit.normal = -unitDir;
	hit.distance = 0.0f;
	return true;	// PT: true indicates a hit, saves some lines in calling code
}

coBool coSweepCapsuleTriangles_Precise(coUint32 nbTris, const coTriangle* coRESTRICT triangles,	// Triangle data
	const coCapsule& capsule,									// Capsule data
	const coVec3& unitDir, const coFloat distance,			// Ray data
	const coUint32* coRESTRICT cachedIndex,					// Cache data
	PxSweepHit& hit, coVec3& triNormalOut,					// Results
	PxHitFlags hitFlags, coBool isDoubleSided,				// Query modifiers
	const BoxPadded* cullBox)								// Cull data
{
	if (!nbTris)
		return false;

	const coBool meshBothSides = hitFlags & PxHitFlag::eMESH_BOTH_SIDES;
	const coBool doBackfaceCulling = !isDoubleSided && !meshBothSides;
	const coBool anyHit = hitFlags & PxHitFlag::eMESH_ANY;
	const coBool testInitialOverlap = !(hitFlags & PxHitFlag::eASSUME_NO_INITIAL_OVERLAP);

	// PT: we can fallback to sphere sweep:
	// - if the capsule is degenerate (i.e. it's a sphere)
	// - if the sweep direction is the same as the capsule axis, in which case we can just sweep the top or bottom sphere

	const coVec3 extrusionDir = (capsule.a - capsule.b) * 0.5f;	// Extrusion dir = capsule segment
	const PxReal halfHeight = coLength(extrusionDir).x;
	bool mustExtrude = halfHeight != 0.0f;
	if (!mustExtrude)
	{
		// PT: capsule is a sphere. Switch to sphere path (intersectCapsuleTriangle doesn't work for degenerate capsules)
		return coSweepSphereTriangles(nbTris, triangles, capsule.a, capsule.radius, unitDir, distance, cachedIndex, hit, triNormalOut, isDoubleSided, meshBothSides, anyHit, testInitialOverlap);
	}
	else
	{
		const coVec3 capsuleAxis = extrusionDir / halfHeight;
		const coFloat colinearity = coAbs(coDot(capsuleAxis, unitDir));
		mustExtrude = (colinearity < (1.0f - COLINEARITY_EPSILON));
	}

	const coVec3 capsuleCenter = (capsule.a + capsule.b) * 0.5f;

	if (!mustExtrude)
	{
		coCapsuleTriangleOverlapData params;
		params.Init(capsule);
		// PT: unfortunately we need to do IO test with the *capsule*, even though we're in the sphere codepath. So we
		// can't directly reuse the sphere function.
		const coVec3 sphereCenter = capsuleCenter + unitDir * halfHeight;
		// PT: this is a copy of 'sweepSphereTriangles' but with a capsule IO test. Saves double backface culling....
		{
			coUint32 index = coUint32(-1);
			const coUint32 initIndex = coGetInitIndex(cachedIndex, nbTris);

			coFloat curT = distance;
			const coFloat dpc0 = coDot(sphereCenter, unitDir);

			coFloat bestAlignmentValue = 2.0f;

			coVec3 bestTriNormal(0.0f);

			for (coUint32 ii = 0; ii < nbTris; ii++)	// We need i for returned triangle index
			{
				const coUint32 i = coGetTriangleIndex(ii, initIndex);

				const coTriangle& currentTri = triangles[i];

				if (coRejectTriangle(sphereCenter, unitDir, curT, capsule.radius, currentTri.verts, dpc0))
					continue;

				coVec3 triNormal;
				currentTri.denormalizedNormal(triNormal);

				// Backface culling
				if (doBackfaceCulling && (coDot(triNormal, unitDir) > 0.0f))
					continue;

				if (testInitialOverlap && coIntersectCapsuleTriangle(triNormal, currentTri.verts[0], currentTri.verts[1], currentTri.verts[2], capsule, params))
				{
					triNormalOut = -unitDir;
					return coSetInitialOverlapResults(hit, unitDir, i);
				}

				const coFloat magnitude = coLength(triNormal).x;
				if (magnitude == 0.0f)
					continue;

				triNormal /= magnitude;

				coFloat currentDistance;
				bool unused;
				if (!coSweepSphereVSTri(currentTri.verts, triNormal, sphereCenter, capsule.radius, unitDir, currentDistance, unused, false))
					continue;

				const coFloat distEpsilon = GU_EPSILON_SAME_DISTANCE; // pick a farther hit within distEpsilon that is more opposing than the previous closest hit
				const coFloat hitDot = coComputeAlignmentValue(triNormal, unitDir);
				if (!coKeepTriangle(currentDistance, hitDot, curT, bestAlignmentValue, distance, distEpsilon))
					continue;

				curT = currentDistance;
				index = i;
				bestAlignmentValue = hitDot;
				bestTriNormal = triNormal;
				if (anyHit)
					break;
			}
			return coComputeSphereTriangleImpactData(hit, triNormalOut, index, curT, sphereCenter, unitDir, bestTriNormal, triangles, isDoubleSided, meshBothSides);
		}
	}

	// PT: extrude mesh on the fly. This is a modified copy of sweepSphereTriangles, unfortunately
	coTriangle extrudedTris[7];
	coVec3 extrudedTrisNormals[7];	// Not normalized

	hit.faceIndex = coUint32(-1);
	const coUint32 initIndex = coGetInitIndex(cachedIndex, nbTris);

	const coFloat radius = capsule.radius;
	coFloat curT = distance;
	const coFloat dpc0 = coDot(capsuleCenter, unitDir).x;

	// PT: we will copy the best triangle here. Using indices alone doesn't work
	// since we extrude on-the-fly (and we don't want to re-extrude later)
	coTriangle bestTri;
	coVec3 bestTriNormal(0.0f);
	coFloat mostOpposingHitDot = 2.0f;

	CapsuleTriangleOverlapData params;
	params.init(capsule);

	for (coUint32 ii = 0; ii < nbTris; ++ii)	// We need i for returned triangle index
	{
		const coUint32 i = coGetTriangleIndex(ii, initIndex);

		const coTriangle& currentSrcTri = triangles[i];	// PT: src tri, i.e. non-extruded

///////////// PT: this part comes from "ExtrudeMesh"
		// Create triangle normal
		coVec3 denormalizedNormal;
		currentSrcTri.denormalizedNormal(denormalizedNormal);

		// Backface culling
		if (doBackfaceCulling && (coDot(denormalizedNormal, unitDir).x > 0.0f))
			continue;

		if (cullBox)
		{
			if (!coIntersectTriangleBox(*cullBox, currentSrcTri.verts[0], currentSrcTri.verts[1], currentSrcTri.verts[2]))
				continue;
		}

		if (testInitialOverlap && coIntersectCapsuleTriangle(denormalizedNormal, currentSrcTri.verts[0], currentSrcTri.verts[1], currentSrcTri.verts[2], capsule, params))
		{
			triNormalOut = -unitDir;
			return coSetInitialOverlapResults(hit, unitDir, i);
		}

		// Extrude mesh on the fly
		coUint32 nbExtrudedTris = 0;

		const coVec3 p0 = currentSrcTri.verts[0] - extrusionDir;
		const coVec3 p1 = currentSrcTri.verts[1] - extrusionDir;
		const coVec3 p2 = currentSrcTri.verts[2] - extrusionDir;

		const coVec3 p0b = currentSrcTri.verts[0] + extrusionDir;
		const coVec3 p1b = currentSrcTri.verts[1] + extrusionDir;
		const coVec3 p2b = currentSrcTri.verts[2] + extrusionDir;

		if (coDot(denormalizedNormal, extrusionDir) >= 0.0f)
			OUTPUT_TRI(p0b, p1b, p2b)
		else
			OUTPUT_TRI(p0, p1, p2)

			// ### it's probably useless to extrude all the shared edges !!!!!
			//if(CurrentFlags & TriangleCollisionFlag::eACTIVE_EDGE12)
		{
			OUTPUT_TRI2(p1, p1b, p2b, unitDir)
			OUTPUT_TRI2(p1, p2b, p2, unitDir)
		}
		//if(CurrentFlags & TriangleCollisionFlag::eACTIVE_EDGE20)
		{
			OUTPUT_TRI2(p0, p2, p2b, unitDir)
			OUTPUT_TRI2(p0, p2b, p0b, unitDir)
		}
		//if(CurrentFlags & TriangleCollisionFlag::eACTIVE_EDGE01)
		{
			OUTPUT_TRI2(p0b, p1b, p1, unitDir)
			OUTPUT_TRI2(p0b, p1, p0, unitDir)
		}
		/////////////

				// PT: TODO: this one is new, to fix the tweak issue. However this wasn't
				// here before so the perf hit should be analyzed.
		denormalizedNormal = coNormalize(denormalizedNormal);
		const coFloat hitDot1 = coComputeAlignmentValue(denormalizedNormal, unitDir);

		for (coUint32 j = 0; j < nbExtrudedTris; j++)
		{
			const coTriangle& currentTri = extrudedTris[j];

			coVec3& triNormal = extrudedTrisNormals[j];
			// Backface culling
			if (doBackfaceCulling && coDot(triNormal, unitDir) > 0.0f)
				continue;

			// PT: beware, culling is only ok on the sphere I think
			if (coRejectTriangle(capsuleCenter, unitDir, curT, radius, currentTri.verts, dpc0))
				continue;

			const coFloat magnitude = coLength(triNormal).x;
			if (magnitude == 0.0f)
				continue;

			triNormal /= magnitude;

			coFloat currentDistance;
			bool unused;
			if (!coSweepSphereVSTri(currentTri.verts, triNormal, capsuleCenter, radius, unitDir, currentDistance, unused, false))
				continue;

			const coFloat distEpsilon = GU_EPSILON_SAME_DISTANCE; // pick a farther hit within distEpsilon that is more opposing than the previous closest hit			
			if (!coKeepTriangle(currentDistance, hitDot1, curT, mostOpposingHitDot, distance, distEpsilon))
				continue;

			curT = currentDistance;
			hit.faceIndex = i;
			mostOpposingHitDot = hitDot1; // arbitrary bias. works for hitDot1=-1, prevHitDot=0
			bestTri = currentTri;
			bestTriNormal = denormalizedNormal;
			if (anyHit)
				goto Exit;	// PT: using goto to have one test per hit, not test per triangle ('break' doesn't work here)
		}
	}
Exit:
	if (hit.faceIndex == coUint32(-1))
		return false;	// We didn't touch any triangle

	hit.distance = curT;

	triNormalOut = bestTriNormal;

	// Compute impact data only once, using best triangle
	coComputeSphereTriImpactData(hit.position, hit.normal, capsuleCenter, unitDir, hit.distance, bestTri);

	// PT: by design, returned normal is opposed to the sweep direction.
	if (coShouldFlipNormal(hit.normal, meshBothSides, isDoubleSided, bestTriNormal, unitDir))
		hit.normal = -hit.normal;

	// PT: revisit this
	if (hit.faceIndex != coUint32(-1))
	{
		// PT: we need to recompute a hit here because the hit between the *capsule* and the source mesh can be very
		// different from the hit between the *sphere* and the extruded mesh.

		// Touched tri
		const coVec3& p0 = triangles[hit.faceIndex].verts[0];
		const coVec3& p1 = triangles[hit.faceIndex].verts[1];
		const coVec3& p2 = triangles[hit.faceIndex].verts[2];

		// AP: measured to be a bit faster than the scalar version
		const coVec3 delta = unitDir * hit.distance;
		coVec3 pointOnSeg, pointOnTri;
		coDistanceSegmentTriangleSquared(
			V3LoadU(capsule.a + delta), V3LoadU(capsule.b + delta),
			V3LoadU(p0), V3LoadU(p1), V3LoadU(p2),
			pointOnSeg, pointOnTri);
		V3StoreU(pointOnTri, hit.position);

		hit.flags = PxHitFlag::eNORMAL | PxHitFlag::ePOSITION;
	}
	return true;
}
