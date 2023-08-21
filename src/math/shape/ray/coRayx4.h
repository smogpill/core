// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../vector/coVec3x4.h"
#include "../../vector/cofloatx4.h"

class coRay2x4
{
public:
	coVec3x4 _origin = coVec3x4(0.0f);
	coVec3x4 _invDir = coVec3x4(0.0f);
	coFloatx4 _length = 0.0f;
};
