// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/quaternion/coQuat.h"
#include "math/vector/coFloatx3.h"

class coTransform
{
public:
	coTransform();
	coTransform(coNullPtr);

	coQuat rotation;
	coFloatx3 translation;
	coFloatx3 scale;
};
