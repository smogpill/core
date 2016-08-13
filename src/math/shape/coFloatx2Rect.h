// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coFloatx2.h"

class coFloatx2Rect
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFloatx2Rect() : pos(0.0f), size(0.0f) {}
	coFloatx2Rect(const coFloatx2& _pos, const coFloatx2& _size) : pos(_pos), size(_size) {}
	coFloatx2 pos;
	coFloatx2 size;
};
