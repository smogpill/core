// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coBool32x4.h"

class alignas(16) coBool32x3x4
{
public:
	coFORCE_INLINE coBool32x3x4() = default;
	coFORCE_INLINE coBool32x3x4(const coBool32x4& x_, const coBool32x4& y_, const coBool32x4& z_) : x(x_), y(y_), z(z_) {}

	coBool32x4 x;
	coBool32x4 y;
	coBool32x4 z;
};
