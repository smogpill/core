// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx3.h"

class coVec3 : public coFloatx3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coVec3() { coBitCast<__m128>(*this) = _mm_setzero_ps(); }
	coFORCE_INLINE coVec3(coFloatx3 _xyz) : coFloatx3(_xyz) {}
	coFORCE_INLINE coVec3(coFloatx3 _xxx, coFloatx3 _yyy, coFloatx3 _zzz) : coFloatx3(_xxx, _yyy, _zzz) {}
	coFORCE_INLINE explicit coVec3(coNullPtr) {}
};
