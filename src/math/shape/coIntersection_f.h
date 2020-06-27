// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3.h"

coFORCE_INLINE coFloat coSigned2DTriArea(const coVec3& a, const coVec3& b, const coVec3& c)
{
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

inline coBool coIntersectSegmentSegment(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& d, coFloat& t)
{
	// 5.1.9.1 2D Segment Intersection, Realtime Collision Detection book, Christer Ericson
	const coFloat a1 = coSigned2DTriArea(a, b, d);
	const coFloat a2 = coSigned2DTriArea(a, b, c);
	if (a1 * a2 < 0.0f)
	{
		const coFloat a3 = coSigned2DTriArea(c, d, a);
		const coFloat a4 = a3 + a2 - a1;
		if (a3 * a4 < 0.0f)
		{
			t = a3 / (a3 - a4);
			return true;
		}
	}

	return false;
}
