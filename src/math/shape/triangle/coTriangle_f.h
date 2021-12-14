// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coTriangle.h"
#include "math/vector/coVec3_f.h"

coFORCE_INLINE coVec3 coGetRawNormal(const coTriangle& t) const
{
	return coCross(t.b - t.a, t.c - t.a);
}
