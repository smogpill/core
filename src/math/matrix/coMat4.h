// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec4.h"

class alignas(16) coMat4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coMat4(coNullPtr) : c0(nullptr), c1(nullptr), c2(nullptr), c3(nullptr) {}
	coFORCE_INLINE coMat4()
		: c0(coBitCast<coVec4>(_mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f)))
		, c1(coBitCast<coVec4>(_mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f)))
		, c2(coBitCast<coVec4>(_mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f)))
		, c3(coBitCast<coVec4>(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f)))
	{}
	coFORCE_INLINE coMat4(coFloatx3 diag)
		: c0(coBitCast<coVec4>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0, 0, 0xffffffff))))
		, c1(coBitCast<coVec4>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0, 0xffffffff, 0))))
		, c2(coBitCast<coVec4>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0, 0xffffffff, 0, 0))))
		, c3(coBitCast<coVec4>(_mm_and_si128(coBitCast<__m128i>(diag), _mm_set_epi32(0xffffffff, 0, 0, 0))))
	{}
	coFORCE_INLINE coMat4(const coFloatx4& _c0, const coFloatx4& _c1, const coFloatx4& _c2, const coFloatx4& _c3) : c0(_c0), c1(_c1), c2(_c2), c3(_c3) {}

	coFORCE_INLINE coVec4& operator[](coUint _i) { return (&c0)[_i]; }
	coFORCE_INLINE const coVec4& operator[](coUint _i) const { return (&c0)[_i]; }

	coVec4 c0;
	coVec4 c1;
	coVec4 c2;
	coVec4 c3;
};
