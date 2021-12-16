// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/shape/capsule/coCapsule.h"
#include "math/vector/coVec3_f.h"

// PT: precomputed data for capsule-triangle test. Useful when testing the same capsule vs several triangles.
class coCapsuleTriangleOverlapData
{
public:
	coVec3 mCapsuleDir;
	coFloat mBDotB;
	coFloat mOneOverBDotB;

	void Init(const coCapsule& capsule)
	{
		const coVec3 dir = capsule.a - capsule.b;
		const coFloat BDotB = coDot(dir, dir).x;
		mCapsuleDir = dir;
		mBDotB = BDotB;
		mOneOverBDotB = BDotB != 0.0f ? 1.0f / BDotB : 0.0f;
	}
};

coBool coIntersectCapsuleTriangle(const coVec3& N, const coVec3& p0, const coVec3& p1, const coVec3& p2, const coCapsule& capsule, const coCapsuleTriangleOverlapData& params);
