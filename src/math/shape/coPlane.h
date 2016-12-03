// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx3.h"

class coPlane
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coPlane() : normal(0.f), distance(0.f) {}

	coFloatx3 normal;
	coFloat distance;
};
