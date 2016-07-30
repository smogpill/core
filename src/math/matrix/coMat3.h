// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx3.h"

class coMat3
{
	coDECLARE_TYPE(coMat3);
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFloatx3 c0;
	coFloatx3 c1;
	coFloatx3 c2;
};
