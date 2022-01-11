// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "math/vector/coVec3.h"

class coPolygon3
{
public:
	coDynamicArray<coVec3> vertices;
};
