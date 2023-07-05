// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
#include "lang/types/coBaseTypes.h"

class alignas(32) coUint64x4
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coUint64x4);
public:
	coFORCE_INLINE coUint64x4() {}
	coFORCE_INLINE coUint64x4(coUint64 x, coUint64 y, coUint64 z, coUint64 w) : x(x), y(y), z(z), w(w) {}
	coFORCE_INLINE coUint64x4(coUint64 xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}

	coFORCE_INLINE coUint64& operator[](coUint i) { coASSERT(i < 4); return (&x)[i]; }
	coFORCE_INLINE const coUint64 operator[](coUint i) const { coASSERT(i < 4); return (&x)[i]; }

	coUint64 x;
	coUint64 y;
	coUint64 z;
	coUint64 w;
};
