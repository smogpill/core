// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx4.h"

class alignas(16) coQuat : public coFloatx4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coQuat() : coFloatx4(0.0f, 0.0f, 0.0f, 1.0f) {}
	coFORCE_INLINE coQuat(const coFloatx4& _xyzw) : coFloatx4(_xyzw) {}
	coFORCE_INLINE coQuat(const coFloatx4& _xxxx, const coFloatx4& _yyyy, const coFloatx4& _zzzz, const coFloatx4& _wwww) : coFloatx4(_xxxx, _yyyy, _zzzz, _wwww) {}
	coFORCE_INLINE explicit coQuat(coNullPtr) {}
};
