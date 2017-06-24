// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/quaternion/coQuat.h"
#include "math/vector/coVec3.h"

class coTransform
{
public:
	coFORCE_INLINE coTransform() : scale(1.0f) {}
	coFORCE_INLINE coTransform(coNullPtr) : rotation(nullptr), translation(nullptr), scale(nullptr) {}

	coQuat rotation;
	coVec3 translation;
	coVec3 scale;
};
