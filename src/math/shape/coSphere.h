// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coVec4.h>

class coSphere
{
public:
	coSphere() = default;
	coSphere(const coVec3& center, const coFloatx4& radius);
	const coVec3& GetCenter() const { return static_cast<const coVec3&>(centerAndRadius); }
	coFloat GetRadius() const { return centerAndRadius.w; }

	coVec4 centerAndRadius;
};
