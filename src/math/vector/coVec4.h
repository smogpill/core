#pragma once
// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx4.h"

class coVec3;

class coVec4 : public coFloatx4
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coFloatx4);
	coDECLARE_SIMD();
public:
	coFORCE_INLINE coVec4() { coBitCast<__m128>(*this) = _mm_setzero_ps(); }
	coFORCE_INLINE coVec4(const coFloat _xyzw) : coFloatx4(_xyzw) {}
	coFORCE_INLINE coVec4(const coFloatx4& _xyzw) : coFloatx4(_xyzw) {}
	coFORCE_INLINE coVec4(const coFloatx4& _xxxx, const coFloatx4& _yyyy, const coFloatx4& _zzzz, const coFloatx4& _wwww) : coFloatx4(_xxxx, _yyyy, _zzzz, _wwww) {}
	explicit coFORCE_INLINE coVec4(const coFloatx3& _xyz) : coFloatx4(_xyz) {}
	coFORCE_INLINE coVec4(const coFloatx3& _xyz, const coFloatx4& _wwww) : coFloatx4(_xyz, _wwww) {}
	coFORCE_INLINE explicit coVec4(coNullPtr) {}

	coFORCE_INLINE operator const coVec3& () const { return reinterpret_cast<const coVec3&>(*this); }
};
