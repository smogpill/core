// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3_f.h"
#include "coAabb.h"
#include "coIntersection_f.h"

inline coFloatx4 coSquareDistancePointSegment(const coVec3& p, const coVec3& a, const coVec3& b)
{
	const coVec3 ab = b - a;
	const coVec3 ap = p - a;
	const coVec3 bp = p - b;
	const coFloatx4 e = coDot(ap, ab);
	if (e <= 0.0f)
		return coSquareLength(ap);
	const coFloatx4 f = coSquareLength(ab);
	if (e >= f)
		return coSquareLength(bp);
	return coSquareLength(ap) - e * e / f;
}

inline coFloatx4 coSquareDistanceSegmentSegment(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& d)
{
	coFloat t;
	if (coIntersectSegmentSegment(a, b, c, d, t))
		return 0.0f;

	coFloatx4 minSquareDist = coSquareDistancePointSegment(a, c, d);
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(b, c, d));
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(c, a, b));
	minSquareDist = coMin(minSquareDist, coSquareDistancePointSegment(d, a, b));
	return minSquareDist;
}

inline coFloatx4 coSquareDistance(const coAabb& a, const coAabb& b)
{
	coAabb outer;
	outer.min = coMin(a.min, b.min);
	outer.max = coMax(a.max, b.max);
	const coVec3 inner = coMax(0.0f, (outer.max - outer.min) - (a.max - a.min) - (b.max - b.min));
	return coSquareLength(inner);
}
