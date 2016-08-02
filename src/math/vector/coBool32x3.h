// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coBool32x3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coUint32 x;
	coUint32 y;
	coUint32 z;
private:
	coUint32 pad;
};
