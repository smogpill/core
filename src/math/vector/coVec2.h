// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx2.h"

class coVec2 : public coFloatx2
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coVec2() { coBitCast<__m128>(*this) = _mm_setzero_ps(); }
	coFORCE_INLINE coVec2(coFloatx2 _xy) : coFloatx2(_xy) {}
	coFORCE_INLINE coVec2(coFloatx2 _xx, coFloatx2 _yy) : coFloatx2(_xx, _yy) {}
	coFORCE_INLINE explicit coVec2(coNullPtr) {}
};
