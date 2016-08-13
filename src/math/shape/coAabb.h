// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx3.h"

class coAabb
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coAabb() : min(0.0f), max(0.0f) {}
	coAabb(const coFloatx3& _min, const coFloatx3& _max) : min(_min), max(_max) {}

	coFloatx3 min;
	coFloatx3 max;
};
