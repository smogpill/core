// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3.h"
#include "math/vector/coFloatx3_f.h"

coFloatx3 coGetScale(const coMat3& _a)
{
	return coMake_Floatx3(coLength(_a.c0), coLength(_a.c1), coLength(_a.c2));
}

coMat3 coRemoveScale(const coMat3& _a)
{
	coMat3 m;
	m.c0 = coNormalize(_a.c0);
	m.c1 = coNormalize(_a.c1);
	m.c2 = coNormalize(_a.c2);
	return m;
}
