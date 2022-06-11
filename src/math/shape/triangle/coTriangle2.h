// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec2.h"

class coTriangle2
{
public:
	coTriangle2() = default;
	coTriangle2(const coVec2& a, const coVec2& b, const coVec2& c) : a(a), b(b), c(c) { }
	coVec2 a;
	coVec2 b;
	coVec2 c;
};

