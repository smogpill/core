// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"

class alignas(16) coUint32x3
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coUint32x3);
	coDECLARE_SIMD();
public:
	coFORCE_INLINE coUint32x3() {}
	coFORCE_INLINE coUint32x3(coUint32 x, coUint32 y, coUint32 z) : x(x), y(y), z(z) {}
	coFORCE_INLINE coUint32x3(coUint32 xyz) : x(xyz), y(xyz), z(xyz) {}
	coFORCE_INLINE explicit coUint32x3(const coFloatx3& f) { coBitCast<__m128i>(*this) = _mm_cvttps_epi32(coBitCast<__m128>(f)); }
	coFORCE_INLINE operator coFloatx3 () const { return coBitCast<coFloatx3>(_mm_cvtepi32_ps(coBitCast<__m128i>(*this))); }
	coUint32 x;
	coUint32 y;
	coUint32 z;
private:
	coUint32 pad;
};
