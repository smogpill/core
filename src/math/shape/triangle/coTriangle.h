// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"

class coTriangle
{
public:
	coTriangle() = default;
	coTriangle(const coVec3& a, const coVec3& b, const coVec3& c) : a(a), b(b), c(c) { }
	coVec3 a;
	coVec3 b;
	coVec3 c;
};

