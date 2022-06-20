// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/shape/coDistance_f.h"

inline coFloat coSquareDistanceSegmentTriangle(const coVec3& vOrigin, const coVec3& vSegment, const coVec3& vP0, const coVec3& vTriEdge0, const coVec3& vTriEdge1)
{
	//=====
	// Impl from Gu::distanceSegmentTriangleSquared(), \PhysX-4.0\physx\source\geomutils\src\distance\GuDistanceSegmentTriangle.cpp
	// Most comments are from the original code.
	// Thanks to the talented people at PhysX for this great piece of work, and for sharing it.
	//=====

	const coFloat fA00 = coSquareLength(vSegment).x;
	const coVec3 vDiff = vP0 - vOrigin;
	const coFloat fA01 = -(coDot(vSegment, vTriEdge0).x);
	const coFloat fA02 = -(coDot(vSegment, vTriEdge1).x);
	const coFloat fA11 = coSquareLength(vTriEdge0).x;
	const coFloat fA12 = coDot(vTriEdge0, vTriEdge1).x;
	const coFloat fA22 = coDot(vTriEdge1, vTriEdge1).x;
	const coFloat fB0 = -(coDot(vDiff, vSegment).x);
	const coFloat fB1 = coDot(vDiff, vTriEdge0).x;
	const coFloat fB2 = coDot(vDiff, vTriEdge1).x;
	const coFloat fCof00 = fA11 * fA22 - fA12 * fA12;
	const coFloat fCof01 = fA02 * fA12 - fA01 * fA22;
	const coFloat fCof02 = fA01 * fA12 - fA02 * fA11;
	const coFloat fDet = fA00 * fCof00 + fA01 * fCof01 + fA02 * fCof02;

	coFloat fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

	// Set up for a relative error test on the angle between ray direction
	// and triangle normal to determine parallel/nonparallel status.
	const coVec3 vNormal = coCross(vTriEdge1, vTriEdge0);
	const coFloat fDot = coDot(vNormal, vSegment).x;
	if (fDot * fDot >= 1e-6f * coSquareLength(vSegment) * coSquareLength(vNormal))
	{
		const coFloat fCof11 = fA00 * fA22 - fA02 * fA02;
		const coFloat fCof12 = fA02 * fA01 - fA00 * fA12;
		const coFloat fCof22 = fA00 * fA11 - fA01 * fA01;
		const coFloat fInvDet = fDet == 0.0f ? 0.0f : 1.0f / fDet;
		const coFloat fRhs0 = -fB0 * fInvDet;
		const coFloat fRhs1 = -fB1 * fInvDet;
		const coFloat fRhs2 = -fB2 * fInvDet;

		fR = fCof00 * fRhs0 + fCof01 * fRhs1 + fCof02 * fRhs2;
		fS = fCof01 * fRhs0 + fCof11 * fRhs1 + fCof12 * fRhs2;
		fT = fCof02 * fRhs0 + fCof12 * fRhs1 + fCof22 * fRhs2;

		if (fR < 0.0f)
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4m
					{
						// minimum on face s=0 or t=0 or r=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3m
					{
						// minimum on face s=0 or r=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5m
				{
					// minimum on face t=0 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 0.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0m
				{
					// minimum on face r=0
					fSqrDist = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 0.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2m
				{
					// minimum on face s=0 or s+t=1 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6m
				{
					// minimum on face t=0 or s+t=1 or r=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1m
				{
					// minimum on face s+t=1 or r=0
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
				fR0 = 0.0f;
				_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
			}
		}
		else if (fR <= 1.0f)
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4
					{
						// minimum on face s=0 or t=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3
					{
						// minimum on face s=0
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
				}
				else if (fT < 0.0f)  // region 5
				{
					// minimum on face t=0
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
				}
				else  // region 0
				{
					// global minimum is interior, done
					fSqrDist = fR * (fA00 * fR + fA01 * fS + fA02 * fT + 2.0f * fB0)
						+ fS * (fA01 * fR + fA11 * fS + fA12 * fT + 2.0f * fB1)
						+ fT * (fA02 * fR + fA12 * fS + fA22 * fT + 2.0f * fB2)
						+ coSquareLength(vDiff).x;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2
				{
					// minimum on face s=0 or s+t=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6
				{
					// minimum on face t=0 or s+t=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1
				{
					// minimum on face s+t=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
			}
		}
		else  // fR > 1
		{
			if (fS + fT <= 1.0f)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4p
					{
						// minimum on face s=0 or t=0 or r=1
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
						fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR0, &fS0);
						fT0 = 0.0f;
						_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
					}
					else  // region 3p
					{
						// minimum on face s=0 or r=1
						fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
						fS = 0.0f;
					}
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 5p
				{
					// minimum on face t=0 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;

					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
					fR0 = 1.0f;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 0p
				{
					// minimum face on r=1
					const coVec3 kPt = vOrigin + vSegment;
					fSqrDist = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS, &fT);
					fR = 1.0f;
				}
			}
			else
			{
				if (fS < 0.0f)  // region 2p
				{
					// minimum on face s=0 or s+t=1 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR, &fT);
					fS = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else if (fT < 0.0f)  // region 6p
				{
					// minimum on face t=0 or s+t=1 or r=1
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
					fT = 0.0f;
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
					fS0 = 1.0f - fT0;
					_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
				}
				else  // region 1p
				{
					// minimum on face s+t=1 or r=1
					const coVec3 kTriSegOrig = vP0 + vTriEdge0;
					const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
					fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR, &fT);
					fS = 1.0f - fT;
				}
				const coVec3 kPt = vOrigin + vSegment;
				fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
				fR0 = 1.0f;
				_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
			}
		}
	}
	else
	{
		// Segment and triangle are parallel
		fSqrDist = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge0, &fR, &fS);
		fT = 0.0f;

		fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, vP0, vTriEdge1, &fR0, &fT0);
		fS0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kTriSegOrig = vP0 + vTriEdge0;
		const coVec3 kTriSegDir = vTriEdge1 - vTriEdge0;
		fSqrDist0 = coSquareDistanceSegmentSegment2(vOrigin, vSegment, kTriSegOrig, kTriSegDir, &fR0, &fT0);
		fS0 = 1.0f - fT0;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		fSqrDist0 = coSquareDistancePointTriangle(vOrigin, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 0.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);

		const coVec3 kPt = vOrigin + vSegment;
		fSqrDist0 = coSquareDistancePointTriangle(kPt, vP0, vTriEdge0, vTriEdge1, &fS0, &fT0);
		fR0 = 1.0f;
		_coUpdateClosestHit(fSqrDist0, fR0, fS0, fT0, fSqrDist, fR, fS, fT);
	}

	// Account for numerical round-off error
	return coMax(0.0f, fSqrDist);
}
