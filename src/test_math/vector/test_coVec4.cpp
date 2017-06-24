// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/vector/coVec4_f.h"

coTEST(coVec4, DefaultConstructor)
{
	coVec4 a;
	coEXPECT(a.x == 0.0f);
	coEXPECT(a.y == 0.0f);
	coEXPECT(a.z == 0.0f);
	coEXPECT(a.w == 0.0f);
}

coTEST(coVec4, Dot)
{
	coEXPECT(coDot(coVec4(), coVec4()) == coVec4());
	coVec4 a = { 0.0f, 1.0f, 2.0f, 3.0f };
	coFloatx4 d = coDot(a, a);
	coEXPECT(coNearEqual(d, 14.0f));
}
