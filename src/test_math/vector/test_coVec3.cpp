// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/vector/coVec3_f.h"

coTEST(coVec3, DefaultConstructor)
{
	coVec3 a;
	coEXPECT(a.x == 0.0f);
	coEXPECT(a.y == 0.0f);
	coEXPECT(a.z == 0.0f);
}

coTEST(coVec3, coDot)
{
	const coVec3 a = { 0.0f, 1.0f, 2.0f };
	const coFloatx4 d = coDot(a, a);
	coEXPECT(coNearEqual(d, 5.0f));
}

coTEST(coVec3, coCross)
{
	coEXPECT(coNearEqual(coCross(coVec3(1, 0, 0), coVec3(0, 1, 0)), coVec3(0, 0, 1)));
	coEXPECT(coNearEqual(coCross(coVec3(0, 1, 0), coVec3(1, 0, 0)), coVec3(0, 0, -1)));
}
