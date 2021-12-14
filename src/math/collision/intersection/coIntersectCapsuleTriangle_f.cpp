// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coIntersectCapsuleTriangle_f.h"
#include "debug/log/coAssert.h"
#include "math/vector/coVec3_f.h"

//=====
// Impl from \physx\source\geomutils\src\intersection\GuIntersectionCapsuleTriangle.cpp from PhysX 4
// Most comments are from the original code.
// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
//=====

// dir = p1 - p0
coFORCE_INLINE coFloat coDistancePointSegmentSquaredInternal(const coVec3& p0, const coVec3& dir, const coVec3& point, coFloat* param = nullptr)
{
	coVec3 diff = point - p0;
	coFloat fT = coDot(diff, dir);

	if (fT <= 0.0f)
	{
		fT = 0.0f;
	}
	else
	{
		const coFloat sqrLen = coSquareLength(dir);
		if (fT >= sqrLen)
		{
			fT = 1.0f;
			diff -= dir;
		}
		else
		{
			fT /= sqrLen;
			diff -= fT * dir;
		}
	}

	if (param)
		*param = fT;

	return coSquareLength(diff);
}

// PT: tests if projections of capsule & triangle overlap on given axis
coFORCE_INLINE coUint32 coTestAxis(const coVec3& p0, const coVec3& p1, const coVec3& p2, const coCapsule& capsule, const coVec3& axis)
{
	// Project capsule
	coFloat min0 = coDot(capsule.a, axis);
	coFloat max0 = coDot(capsule.b, axis);
	if (min0 > max0)
		coSwap(min0, max0);
	const coFloat MR = coLength(axis).x * capsule.radius;
	min0 -= MR;
	max0 += MR;

	// Project triangle
	coFloat min1, max1;
	{
		min1 = max1 = coDot(p0, axis).x;
		coFloat dp = coDot(p1, axis);
		if (dp < min1)	min1 = dp;
		if (dp > max1)	max1 = dp;
		dp = coDot(p2, axis);
		if (dp < min1)	min1 = dp;
		if (dp > max1)	max1 = dp;
	}

	// Test projections
	if (max0 < min1 || max1 < min0)
		return 0;

	return 1;
}

// PT: computes shortest vector going from capsule axis to triangle edge
coFORCE_INLINE coVec3 coComputeEdgeAxis(const coVec3& p, const coVec3& a,
	const coVec3& q, const coVec3& b,
	coFloat BDotB, coFloat oneOverBDotB)
{
	const coVec3 T = q - p;
	const coFloat ADotA = coDot(a, a);
	const coFloat ADotB = coDot(a, b);
	const coFloat ADotT = coDot(a, T);
	const coFloat BDotT = coDot(b, T);

	const coFloat denom = ADotA * BDotB - ADotB * ADotB;

	coFloat t = denom != 0.0f ? (ADotT * BDotB - BDotT * ADotB) / denom : 0.0f;
	t = coClamp(t, 0.0f, 1.0f);

	coFloat u = (t * ADotB - BDotT) * oneOverBDotB;

	if (u < 0.0f)
	{
		u = 0.0f;
		t = ADotT / ADotA;
		t = coClamp(t, 0.0f, 1.0f);
	}
	else if (u > 1.0f)
	{
		u = 1.0f;
		t = (ADotB + ADotT) / ADotA;
		t = coClamp(t, 0.0f, 1.0f);
	}
	return T + b * u - a * t;
}

coBool coIntersectCapsuleTriangle(const coVec3& N, const coVec3& p0, const coVec3& p1, const coVec3& p2, const coCapsule& capsule, const coCapsuleTriangleOverlapData& params)
{
	coASSERT(capsule.a != capsule.b);

	{
		const coFloat d2 = coDistancePointSegmentSquaredInternal(capsule.a, params.mCapsuleDir, p0);
		if (d2 <= capsule.radius * capsule.radius)
			return true;
	}

	//	const PxVec3 N = (p0 - p1).cross(p0 - p2);

	if (!coTestAxis(p0, p1, p2, capsule, N))
		return false;

	if (!coTestAxis(p0, p1, p2, capsule, coComputeEdgeAxis(p0, p1 - p0, capsule.a, params.mCapsuleDir, params.mBDotB, params.mOneOverBDotB)))
		return false;

	if (!coTestAxis(p0, p1, p2, capsule, coComputeEdgeAxis(p1, p2 - p1, capsule.a, params.mCapsuleDir, params.mBDotB, params.mOneOverBDotB)))
		return false;

	if (!coTestAxis(p0, p1, p2, capsule, coComputeEdgeAxis(p2, p0 - p2, capsule.a, params.mCapsuleDir, params.mBDotB, params.mOneOverBDotB)))
		return false;

	return true;
}
