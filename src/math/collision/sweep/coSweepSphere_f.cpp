// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSweepSphere_f.h"
#include "math/vector/coVec3_f.h"
#include "math/shape/coDistance_f.h"
#include "math/shape/coIntersection_f.h"
#include "math/collision/intersection/coIntersectRaySphere_f.h"
#include "math/collision/intersection/coIntersectRayCapsule_f.h"

// Comment from physx\source\geomutils\src\sweep\GuSweepSphereTriangle.cpp, from Physx 4:
// PT: using GU_CULLING_EPSILON_RAY_TRIANGLE fails here, in capsule-vs-mesh's triangle extrusion, when
// the sweep dir is almost the same as the capsule's dir (i.e. when we usually fallback to the sphere codepath).
// I suspect det becomes so small that we lose all accuracy when dividing by det and using the result in computing
// impact distance.
#define coLOCAL_EPSILON 0.00001f

// PT: special version computing (u,v) even when the ray misses the tri. Version working on precomputed edges.
static coFORCE_INLINE coUint32 rayTriSpecial(const coVec3& orig, const coVec3& dir, const coVec3& vert0, const coVec3& edge1, const coVec3& edge2, coFloat& t, coFloat& u, coFloat& v)
{
	//=====
	// Impl from rayTriSpecial(), \physx\source\geomutils\src\sweep\GuSweepSphereTriangle.cpp, from Physx 4
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	// Begin calculating determinant - also used to calculate U parameter
	const coVec3 pvec = coCross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	const coFloat det = coDot(edge1, pvec).x;

	// the non-culling branch
//	if(det>-GU_CULLING_EPSILON_RAY_TRIANGLE && det<GU_CULLING_EPSILON_RAY_TRIANGLE)
	if (det > -coLOCAL_EPSILON && det < coLOCAL_EPSILON)
		return 0;
	const coFloat oneOverDet = 1.0f / det;

	// Calculate distance from vert0 to ray origin
	const coVec3 tvec = orig - vert0;

	// Calculate U parameter
	u = coDot(tvec, pvec).x * oneOverDet;

	// prepare to test V parameter
	const coVec3 qvec = coCross(tvec, edge1);

	// Calculate V parameter
	v = coDot(dir, qvec).x * oneOverDet;

	if (u < 0.0f || u>1.0f)
		return 1;
	if (v < 0.0f || u + v>1.0f)
		return 1;

	// Calculate t, ray intersects triangle
	t = coDot(edge2, qvec).x * oneOverDet;

	return 2;
}

// Returns true if sphere can be tested against triangle vertex, false if edge test should be performed
//
// Uses a conservative approach to work for "sliver triangles" (long & thin) as well.
static coFORCE_INLINE bool coEdgeOrVertexTest(const coVec3& planeIntersectPoint, const coVec3* coRESTRICT tri, coUint32 vertIntersectCandidate, coUint32 vert0, coUint32 vert1, coUint32& secondEdgeVert)
{
	//=====
	// Impl from edgeOrVertexTest(), \physx\source\geomutils\src\sweep\GuSweepSphereTriangle.cpp, from Physx 4
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	{
		const coVec3 edge0 = tri[vertIntersectCandidate] - tri[vert0];
		const coFloat edge0LengthSqr = coSquareLength(edge0).x;

		const coVec3 diff = planeIntersectPoint - tri[vert0];

		if (coDot(edge0, diff) < edge0LengthSqr)  // If the squared edge length is used for comparison, the edge vector does not need to be normalized
		{
			secondEdgeVert = vert0;
			return false;
		}
	}

	{
		const coVec3 edge1 = tri[vertIntersectCandidate] - tri[vert1];
		const coFloat edge1LengthSqr = coSquareLength(edge1).x;

		const coVec3 diff = planeIntersectPoint - tri[vert1];

		if (coDot(edge1, diff) < edge1LengthSqr)
		{
			secondEdgeVert = vert1;
			return false;
		}
	}
	return true;
}

static coFORCE_INLINE coBool coTestRayVsSphereOrCapsule(coFloat& impactDistance, coBool testSphere, const coVec3& center, coFloat radius, const coVec3& dir, const coVec3* coRESTRICT verts, coUint32 e0, coUint32 e1)
{
	if (testSphere)
	{
		coFloat t;
		if (coIntersectRaySphere(center, dir, FLT_MAX, verts[e0], radius, t))
		{
			impactDistance = t;
			return true;
		}
	}
	else
	{
		coFloat t;
		if (coIntersectRayCapsule(center, dir, verts[e0], verts[e1], radius, t))
		{
			if (t >= 0.0f/* && t<MinDist*/)
			{
				impactDistance = t;
				return true;
			}
		}
	}
	return false;
}

coBool coSweepSphereTriangle(const coVec3* coRESTRICT triVerts, const coVec3& normal, const coVec3& center, coFloat radius, const coVec3& dir, coFloat& impactDistance, coBool& directHit, coBool testInitialOverlap)
{
	//=====
	// Impl from Gu::sweepSphereVSTri(), \physx\source\geomutils\src\sweep\GuSweepSphereTriangle.cpp, from Physx 4
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	directHit = false;
	const coVec3 edge10 = triVerts[1] - triVerts[0];
	const coVec3 edge20 = triVerts[2] - triVerts[0];

	if(testInitialOverlap)	// ### brute force version that always works, but we can probably do better
	{
		const coVec3 cp = coGetClosestPointPointTriangle2(center, triVerts[0], triVerts[1], triVerts[2], edge10, edge20);
		if(coSquareLength(cp - center) <= radius*radius)
		{
			impactDistance = 0.0f;
			return true;
		}
	}

	#define INTERSECT_POINT (triVerts[1]*u) + (triVerts[2]*v) + (triVerts[0] * (1.0f-u-v))

	coFloat u,v;
	{
		coVec3 R = normal * radius;
		if(coDot(dir, R) >= 0.0f)
			R = -R;

		// The first point of the sphere to hit the triangle plane is the point of the sphere nearest to
		// the triangle plane. Hence, we use center - (normal*radius) below.

		// PT: casting against the extruded triangle in direction R is the same as casting from a ray moved by -R
		coFloat t;
		const coUint32 r = rayTriSpecial(center-R, dir, triVerts[0], edge10, edge20, t, u, v);
		if(!r)
			return false;
		if(r==2)
		{
			if(t<0.0f)
				return false;
			impactDistance = t;
			directHit = true;
			return true;
		}
	}

	//
	// Let's do some art!
	//
	// The triangle gets divided into the following areas (based on the barycentric coordinates (u,v)):
	//
	//               \   A0    /
	//                 \      /
	//                   \   /
	//                     \/ 0
	//            A02      *      A01
	//   u /              /   \          \ v
	//    *              /      \         *
	//                  /         \						.
	//               2 /            \ 1
	//          ------*--------------*-------
	//               /                 \				.
	//        A2    /        A12         \   A1
	//
	//
	// Based on the area where the computed triangle plane intersection point lies in, a different sweep test will be applied.
	//
	// A) A01, A02, A12  : Test sphere against the corresponding edge
	// B) A0, A1, A2     : Test sphere against the corresponding vertex
	//
	// Unfortunately, B) does not work for long, thin triangles. Hence there is some extra code which does a conservative check and
	// switches to edge tests if necessary.
	//

	coBool TestSphere;
	coUint32 e0,e1;
	if(u < 0.0f)
	{
		if(v < 0.0f)
		{
			// 0 or 0-1 or 0-2
			e0 = 0;
			const coVec3 intersectPoint = INTERSECT_POINT;
			TestSphere = coEdgeOrVertexTest(intersectPoint, triVerts, 0, 1, 2, e1);
		}
		else if(u+v>1.0f)
		{
			// 2 or 2-0 or 2-1
			e0 = 2;
			const coVec3 intersectPoint = INTERSECT_POINT;
			TestSphere = coEdgeOrVertexTest(intersectPoint, triVerts, 2, 0, 1, e1);
		}
		else
		{
			// 0-2
			TestSphere = false;
			e0 = 0;
			e1 = 2;
		}
	}
	else
	{
		if(v<0.0f)
		{
			if(u+v>1.0f)
			{
				// 1 or 1-0 or 1-2
				e0 = 1;
				const coVec3 intersectPoint = INTERSECT_POINT;
				TestSphere = coEdgeOrVertexTest(intersectPoint, triVerts, 1, 0, 2, e1);
			}
			else
			{
				// 0-1
				TestSphere = false;
				e0 = 0;
				e1 = 1;
			}
		}
		else
		{
			coASSERT(u+v>=1.0f);	// Else hit triangle
			// 1-2
			TestSphere = false;
			e0 = 1;
			e1 = 2;
		}
	}
	return coTestRayVsSphereOrCapsule(impactDistance, TestSphere, center, radius, dir, triVerts, e0, e1);
}
