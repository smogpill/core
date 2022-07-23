// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "coMat3.h"

class alignas(16) coSymMat3
{
	coDECLARE_CLASS_NO_POLYMORPHISM();
public:
	coFORCE_INLINE coSymMat3(coNullPtr) {}
	coFORCE_INLINE coSymMat3(coFloat d = 1.0f)
		: m00(d), m01(0.0f), m02(0.0f), m11(d), m12(0.0f), m22(d)
	{

	}
	coFORCE_INLINE coSymMat3(coFloat m00, coFloat m01, coFloat m02, coFloat m11, coFloat m12, coFloat m22)
		: m00(m00), m01(m01), m02(m02), m11(m11), m12(m12), m22(m22)
	{
	}
	coFORCE_INLINE coSymMat3(const coMat3& m)
		: m00(m.c0.x), m01(m.c0.y), m02(m.c0.z), m11(m.c1.y), m12(m.c1.z), m22(m.c2.z)
	{

	}

	coFloat m00, m01, m02, m11, m12, m22;
};
