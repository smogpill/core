// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coDynamicArrayx3.h"
#include "coDynamicArray_f.h"

template <class T>
void coResize(coDynamicArrayx3<T>& a, const coInt32x3 count)
{
	coResize(a.data, count.x * count.y * count.z);
	a.count = count;
}

template <class T>
coDynamicArrayx3<T>::coDynamicArrayx3(const coInt32x3& count)
{
	coResize(*this, count);
}
