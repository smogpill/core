// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

template <class T>
void coReserve(coDynamicArray<T>& _array, coUint32 _size)
{
	if (_array.capacity < _size)
	{
	}
}

template <class T>
void coAdd(coDynamicArray<T>& _array, const T& _val)
{
	coUint32 newCount = _array.count + 1;
	coReserve(_array, newCount);
	_array.data[_array.count] = _val;
	_array.count = newCount;
}
/*

template <class T>
void coRemoveUnordered(coDynamicArray<T>& _array, )
{
}
*/
