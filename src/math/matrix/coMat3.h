// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec3.h"

class alignas(16) coMat3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coMat3() {}
	coFORCE_INLINE coMat3(coFloatx3 xyz) : c0(xyz), c1(xyz), c2(xyz) {}
	coFORCE_INLINE coMat3(coFloatx3 x, coFloatx3 y, coFloatx3 z) : c0(x), c1(y), c2(z) {}
	coFORCE_INLINE coMat3(coNullPtr) : c0(nullptr), c1(nullptr), c2(nullptr) {}

	coFORCE_INLINE coVec3& operator[](coUint _i) { return (&c0)[_i]; }
	coFORCE_INLINE const coVec3& operator[](coUint _i) const { return (&c0)[_i]; }

	coVec3 c0;
	coVec3 c1;
	coVec3 c2;
};
