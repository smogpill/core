// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec3.h"

class coMat3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coMat3() {}
	coFORCE_INLINE coMat3(coNullPtr) : c0(nullptr), c1(nullptr), c2(nullptr) {}

	coVec3 c0;
	coVec3 c1;
	coVec3 c2;
};
