// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/coInt32x2.h"

class coInt32x2Rect
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coInt32x2Rect);
public:
	coInt32x2Rect() : pos(0), size(0) {}
	coInt32x2Rect(const coInt32x2& _pos, const coInt32x2& _size) : pos(_pos), size(_size) {}
	coInt32x2 pos;
	coInt32x2 size;
};
