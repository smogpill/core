// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coFloatx4.h"

class alignas(16) coVec3x4
{
	coDECLARE_SIMD();
public:
	coFORCE_INLINE coVec3x4() : coVec3x4(coBitCast<coFloatx4>(_mm_setzero_ps())) {}
	coFORCE_INLINE coVec3x4(const coFloatx4& xyz) : x(xyz), y(xyz), z(xyz) {}
	coFORCE_INLINE coVec3x4(const coFloatx4& x_, const coFloatx4& y_, const coFloatx4& z_) : x(x_), y(y_), z(z_) {}
	coFORCE_INLINE explicit coVec3x4(coNullPtr) {}

	coFloatx4 x;
	coFloatx4 y;
	coFloatx4 z;
};

/*
class coVec3Splatx4 : public coVec3
{
public:
	using coVec3::coVec3;
};
*/