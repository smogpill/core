// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx3.h"

class coSegment
{
public:
	coSegment() : p0(0.f), p1(0.f) {}
	coSegment(const coFloatx3& _p0, const coFloatx3& _p1) : p0(_p0), p1(_p1) {}
	coFloatx3 p0;
	coFloatx3 p1;
};
