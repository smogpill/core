// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coBool32x3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coBool32 x;
	coBool32 y;
	coBool32 z;
private:
	coBool32 pad;
};
