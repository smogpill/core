// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coInt32x3.h>
#include "coDynamicArray.h"

template <class T>
class coDynamicArrayx3
{
public:
	coDynamicArrayx3() = default;
	coDynamicArrayx3(const coInt32x3& count);
	coInt32x3 count = 0;
	coDynamicArray<T> data;
};
