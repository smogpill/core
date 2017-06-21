// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx3.h"

class alignas(16) coVec3 : public coFloatx3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coVec3() { *this = coBitCast<coVec3>(_mm_setzero_ps()); }
	coFORCE_INLINE coVec3(coFloat _xyz) : coFloatx3(_xyz) {}
	coFORCE_INLINE coVec3(coFloatx3 _xyz) : coFloatx3(_xyz) {}
	coFORCE_INLINE coVec3(coFloat _x, coFloat _y, coFloat _z) : coFloatx3(_x, _y, _z) {}
	explicit coVec3(coNullPtr);
};
