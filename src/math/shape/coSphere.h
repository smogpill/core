// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coVec4.h>

class coSphere
{
public:
	coSphere() = default;
	coSphere(const coVec3& center, const coFloatx4& radius);
	coVec4 centerAndRadius;
};
