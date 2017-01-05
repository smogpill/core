// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx4.h"

class coPlane
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coPlane() : normalAndDistance(0.f) {}
	coPlane(const coFloatx4& _normalAndDistance) : normalAndDistance(_normalAndDistance) {}

	coFloatx4 normalAndDistance;
};
