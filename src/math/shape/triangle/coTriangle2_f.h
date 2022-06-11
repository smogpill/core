// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coTriangle2.h"

coFORCE_INLINE coFloat coSign(const coTriangle2& t)
{
	return (t.a.x - t.c.x) * (t.b.y - t.c.y) - (t.b.x - t.c.x) * (t.a.y - t.c.y);
}

coFORCE_INLINE coBool coOverlap(const coTriangle2& t, const coVec2& p)
{
	// https://www.gamedev.net/forums/topic.asp?topic_id=295943
	const coFloat d1 = coSign(coTriangle2(p, t.a, t.b));
	const coFloat d2 = coSign(coTriangle2(p, t.b, t.c));
	const coFloat d3 = coSign(coTriangle2(p, t.c, t.a));

	const coBool hasNeg = (d1 < 0) | (d2 < 0) | (d3 < 0);
	const coBool hasPos = (d1 > 0) | (d2 > 0) | (d3 > 0);

	return !(hasNeg & hasPos);
}