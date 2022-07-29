// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coInt32x2
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coInt32x2);
public:
	coFORCE_INLINE coInt32x2() {}
	coFORCE_INLINE coInt32x2(coInt32 _xy) : x(_xy), y(_xy) {}
	coFORCE_INLINE coInt32x2(coInt32 _x, coInt32 _y) : x(_x), y(_y) {}
	coFORCE_INLINE coInt32& operator[](coUint _i) { return (&x)[_i]; }
	coFORCE_INLINE coInt32 operator[](coUint _i) const { return (&x)[_i]; }

	coInt32 x;
	coInt32 y;
private:
	coInt32 pad0;
	coInt32 pad1;
};
