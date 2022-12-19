// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"

class coUint32x2
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coUint32x2);
public:
	coFORCE_INLINE coUint32x2(coUint32 x, coUint32 y) : x(x), y(y) {}
	coFORCE_INLINE coUint32x2(coUint32 xy) : x(xy), y(xy) {}
	coUint32 x;
	coUint32 y;
};
