// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coUint32x2
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coUint32x2(coUint32 x, coUint32 y) : x(x), y(y) {}
	coFORCE_INLINE coUint32x2(coUint32 xy) : x(xy), y(xy) {}
	coUint32 x;
	coUint32 y;
private:
	coUint32 pad0;
	coUint32 pad1;
};
