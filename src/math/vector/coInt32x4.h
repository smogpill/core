// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coInt32x4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coInt32 x;
	coInt32 y;
	coInt32 z;
	coInt32 w;
};
