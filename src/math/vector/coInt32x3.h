// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
class coFloatx3;

class alignas(16) coInt32x3
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coInt32x3);
public:
	coFORCE_INLINE coInt32x3() {}
	coFORCE_INLINE coInt32x3(coInt32 _xyz) : x(_xyz), y(_xyz), z(_xyz) {}
	coFORCE_INLINE coInt32x3(coInt32 _x, coInt32 _y, coInt32 _z) : x(_x), y(_y), z(_z) {}
	coFORCE_INLINE explicit coInt32x3(const coFloatx3& f) { coBitCast<__m128i>(*this) = _mm_cvttps_epi32(coBitCast<__m128>(f)); }
	coFORCE_INLINE coInt32& operator[](coUint _i) { return (&x)[_i]; }
	coFORCE_INLINE coInt32 operator[](coUint _i) const { return (&x)[_i]; }

	coInt32 x;
	coInt32 y;
	coInt32 z;
private:
	coInt32 pad;
};
