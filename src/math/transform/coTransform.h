// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/quaternion/coQuat.h"
#include "math/vector/coVec3.h"
class coMat4;

class alignas(16) coTransform
{
public:
	coFORCE_INLINE coTransform() : scale(1.0f) {}
	coFORCE_INLINE coTransform(coNullPtr) : rotation(nullptr), translation(nullptr), scale(nullptr) {}
	explicit coTransform(const coMat4&);
	
	static const coTransform identity;
	coQuat rotation;
	coVec3 translation;
	coVec3 scale;
};
