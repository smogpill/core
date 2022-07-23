// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "coMat4.h"

class alignas(16) coSymMat4
{
	coDECLARE_CLASS_NO_POLYMORPHISM();
public:
	coFORCE_INLINE coSymMat4(coNullPtr) {}
	coFORCE_INLINE coSymMat4(coFloat d = 1.0f)
		: m00(d), m01(0.0f), m02(0.0f), m03(0.0f), m11(d), m12(0.0f), m13(0.0f), m22(d), m23(0.0f), m33(d)
	{

	}
	coFORCE_INLINE coSymMat4(coFloat m00, coFloat m01, coFloat m02, coFloat m03, coFloat m11, coFloat m12, coFloat m13, coFloat m22, coFloat m23, coFloat m33)
		: m00(m00), m01(m01), m02(m02), m03(m03), m11(m11), m12(m12), m13(m13), m22(m22), m23(m23), m33(m33)
	{
	}
	coFORCE_INLINE coSymMat4(const coMat4& m)
		: m00(m.c0.x), m01(m.c0.y), m02(m.c0.z), m03(m.c0.w)
		, m11(m.c1.y), m12(m.c1.z), m13(m.c1.w)
		, m22(m.c2.z), m23(m.c2.w)
		, m33(m.c3.w)
	{

	}

	coFloat m00, m01, m02, m03, m11, m12, m13, m22, m23, m33;
};
