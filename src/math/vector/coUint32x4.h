// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
#include <debug/log/coAssert.h>

class alignas(16) coUint32x4
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coUint32x4);
public:
	coFORCE_INLINE coUint32x4() {}
	coFORCE_INLINE coUint32x4(coUint32 x, coUint32 y, coUint32 z, coUint32 w) : x(x), y(y), z(z), w(w) {}
	coFORCE_INLINE coUint32x4(coUint32 xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}

	coFORCE_INLINE coUint32& operator[](coUint idx) { coASSERT(idx < 4); return (&x)[idx]; }
	coFORCE_INLINE coUint32 operator[](coUint idx) const { coASSERT(idx < 4); return (&x)[idx]; }

	coUint32 x;
	coUint32 y;
	coUint32 z;
	coUint32 w;
};
