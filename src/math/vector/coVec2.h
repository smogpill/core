// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
#include "coFloatx2.h"
#include "coUint32x2.h"

class coVec2 : public coFloatx2
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coFloatx2);
	coDECLARE_SIMD();
public:
	coFORCE_INLINE coVec2() { coBitCast<__m128>(*this) = _mm_setzero_ps(); }
	coFORCE_INLINE coVec2(const coFloatx2& _xy) : coFloatx2(_xy) {}
	coFORCE_INLINE coVec2(const coFloatx2& _xx, const coFloatx2& _yy) : coFloatx2(_xx, _yy) {}
	coFORCE_INLINE explicit coVec2(const coUint32x2& xy) : coVec2(coFloat(xy.x), coFloat(xy.y)) {}
	coFORCE_INLINE explicit coVec2(coNullPtr) {}
};
