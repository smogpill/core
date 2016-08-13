// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coFloatx2
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coFloatx2(){}
	coFORCE_INLINE coFloatx2(coFloat _xy) : x(_xy), y(_xy) {}
	coFORCE_INLINE coFloatx2(coFloat _x, coFloat _y) : x(_x), y(_y) {}

	coFloat x;
	coFloat y;
private:
	coFloat pad0;
	coFloat pad1;
};
