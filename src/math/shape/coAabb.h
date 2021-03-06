// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coVec3.h"

class coAabb
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coAabb() {}
	coAabb(coNullPtr) : min(nullptr), max(nullptr) {}
	coAabb(const coVec3& _min, const coVec3& _max) : min(_min), max(_max) {}

	coVec3 min;
	coVec3 max;
};
