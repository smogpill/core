// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"
#include "container/array/coArray_f.h"
#include "math/scalar/coUint32_f.h"

template<class T>
coDynamicArray<T>::~coDynamicArray()
{
	allocator->free(data);
}

template <class T>
void coReserve(coDynamicArray<T>& _this, coUint32 _desiredCount)
{
	if (_desiredCount > _this.capacity)
	{
		const coUint32 clampedCapacity = coMax(16u, _desiredCount);
		coASSERT(_array.allocator);
		T* newBuffer = static_cast<T*>(_this.allocator->allocate(clampedCapacity * sizeof(T)));
		if (_this.data) // [opt] not using size directly not to break CPU's pipe too early
		{
			coMemCopy(newBuffer, _this.data, _this.count * sizeof(T));
			_this.allocator->free(_this.data);
		}
		_this.data = newBuffer;
		_this.capacity = clampedCapacity;
	}
}

template <class T>
void coPushBack(coDynamicArray<T>& _array, const T& _val)
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
