// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/vector/coFloatx4_f.h"

coTEST(coFloatx4, Dot)
{
	coFloatx4 a = {0.0f, 1.0f, 2.0f, 3.0f};
	coFloatx4 d = coDot(a, a);
}
