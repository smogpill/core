// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"
#include "container/array/coArray_f.h"
#include "container/array/coConstArray_f.h"
#include "memory/allocator/coAllocator.h"
#include "lang/coCppExtensions.h"

template <class T>
coDynamicArray<T>::coDynamicArray()
	: coDynamicArray(*coAllocator::GetHeap())
{
}

template <class T>
coDynamicArray<T>::coDynamicArray(coAllocator& _allocator)
	: capacity(0)
	, allocator(&_allocator)
{
	static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");
}

template<class T>
coDynamicArray<T>::~coDynamicArray()
{
	allocator->Free(data);
}

template <class T>
void coReserve(coDynamicArray<T>& _this, coUint32 _desiredCount)
{
	if (_desiredCount > _this.capacity)
	{
		const coUint32 clampedCapacity = _desiredCount >= 16u ? _desiredCount : 16u;
		coASSERT(_this.allocator);
		T* newBuffer = static_cast<T*>(_this.allocator->Allocate(clampedCapacity * sizeof(T)));
		if (_this.data) // [opt] not using size directly not to break CPU's pipe too early
		{
			coMemCopy(newBuffer, _this.data, _this.count * sizeof(T));
			_this.allocator->Free(_this.data);
		}
		_this.data = newBuffer;
		_this.capacity = clampedCapacity;
	}
}

template <class T>
void coPushBack(coDynamicArray<T>& _this, const T& _val)
{
	coUint32 newCount = _this.count + 1;
	coReserve(_this, newCount);
	_this.data[_this.count] = _val;
	_this.count = newCount;
}

template <class T>
void coPushBackArray(coDynamicArray<T>& _this, const coConstArray<T>& _from)
{
	//static_assert(std::is_base_of<coArray<T>, A>::value, "_this should be an array");
	const coUint32 desiredCount = _this.count + _from.count;
	if (desiredCount > _this.capacity)
	{
		const coUint32 newCapacity = desiredCount > 8u ? desiredCount : 8u;
		coReserve(_this, newCapacity);
	}
	coMemCopy(&_this.data[_this.count], _from.data, _from.count);
	_this.count += _from.count;
}

template <class T>
coDynamicArray<T>::coDynamicArray(const coDynamicArray<T>& _)
	: coDynamicArray()
{
	operator=(_);
}

template <class T>
coDynamicArray<T>& coDynamicArray<T>::operator=(const coDynamicArray<T>& _)
{
	coReserve(*this, _.count);
	coMemCopy(data, _.data, _.count);
	count = _.count;
	return *this;
}

/*

template <class T>
void coRemoveUnordered(coDynamicArray<T>& _array, )
{
}
*/
