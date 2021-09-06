// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coUint32x4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coUint32x4() {}
	coFORCE_INLINE coUint32x4(coUint32 x, coUint32 y, coUint32 z, coUint32 w) : x(x), y(y), z(z), w(w) {}
	coFORCE_INLINE coUint32x4(coUint32 xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	coUint32 x;
	coUint32 y;
	coUint32 z;
	coUint32 w;
};
