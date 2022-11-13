// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec3.h"

class alignas(16) coMat3
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coMat3);
public:
	coFORCE_INLINE coMat3(coNullPtr) : c0(nullptr), c1(nullptr), c2(nullptr) {}
	coFORCE_INLINE explicit coMat3(coFloat d = 1.0f)
		: c0(coBitCast<coVec3>(_mm_set_ps(0.0f, 0.0f, 0.0f, d)))
		, c1(coBitCast<coVec3>(_mm_set_ps(0.0f, 0.0f, d, 0.0f)))
		, c2(coBitCast<coVec3>(_mm_set_ps(0.0f, d, 0.0f, 0.0f)))
	{}
	coFORCE_INLINE explicit coMat3(coFloatx3 diag) 
		: c0(coBitCast<coVec3>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0, 0, 0xffffffff))))
		, c1(coBitCast<coVec3>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0, 0xffffffff, 0))))
		, c2(coBitCast<coVec3>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0xffffffff, 0, 0))))
	{}
	coFORCE_INLINE coMat3(coFloatx3 x, coFloatx3 y, coFloatx3 z) : c0(x), c1(y), c2(z) {}

	coFORCE_INLINE coVec3& operator[](coUint _i) { return (&c0)[_i]; }
	coFORCE_INLINE const coVec3& operator[](coUint _i) const { return (&c0)[_i]; }

	coVec3 c0;
	coVec3 c1;
	coVec3 c2;
};
