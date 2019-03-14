// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec4.h"

class alignas(16) coMat4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coMat4(coNullPtr) : c0(nullptr), c1(nullptr), c2(nullptr), c3(nullptr) {}
	coFORCE_INLINE coMat4() : c0(1.0f, 0.0f, 0.0f, 0.0f), c1(0.0f, 1.0f, 0.0f, 0.0f), c2(0.0f, 0.0f, 1.0f, 0.0f), c3(0.0f, 0.0f, 0.0f, 1.0f) {}
	coFORCE_INLINE coMat4(const coVec4& _c0, const coVec4& _c1, const coVec4& _c2, const coVec4& _c3) : c0(_c0), c1(_c1), c2(_c2), c3(_c3) {}

	coVec4 c0;
	coVec4 c1;
	coVec4 c2;
	coVec4 c3;
};
