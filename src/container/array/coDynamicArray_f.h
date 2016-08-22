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
	coASSERT(allocator);
	allocator->FreeAligned(data);
}

coUint32 _coComputeBestArrayCapacity(coUint32 _capacity);

template <class T>
void coReserve(coDynamicArray<T>& _this, coUint32 _desiredCount)
{
	if (_desiredCount > _this.capacity)
	{
		const coUint32 bestCapacity = _coComputeBestArrayCapacity(_desiredCount);
		coASSERT(_this.allocator);
		T* newBuffer = static_cast<T*>(_this.allocator->AllocateAligned(bestCapacity * sizeof(T),  alignof(T)));
		if (_this.data)
		{
			coMemCopy(newBuffer, _this.data, _this.count * sizeof(T));
			_this.allocator->FreeAligned(_this.data);
		}
		_this.data = newBuffer;
		_this.capacity = bestCapacity;
	}
}

template <class T, class U>
T& coPushBack(coDynamicArray<T>& _this, const U& _val)
{
	coUint32 newCount = _this.count + 1;
	coReserve(_this, newCount);
	T& e = _this.data[_this.count];
	e = _val;
	_this.count = newCount;
	return e;
}

template <class T>
void coPushBackArray(coDynamicArray<T>& _this, const coArray<const T>& _other)
{
	//static_assert(std::is_base_of<coArray<T>, A>::value, "_this should be an array");
	const coUint32 desiredCount = _this.count + _other.count;
	if (desiredCount > _this.capacity)
		coReserve(_this, desiredCount);
	coMemCopy(&_this.data[_this.count], _other.data, _other.count * sizeof(T));
	_this.count = desiredCount;
	
}

template <class T>
void coPushBackArray(coDynamicArray<T>& _this, const coArray<T>& _other)
{
	coPushBackArray(_this, static_cast<const coArray<const T>&>(_other));
}

template <class T>
void coSwap(coDynamicArray<T>& _this, coDynamicArray<T>& _other)
{
	coSwapMemory(_this, _other);
}

template <class T>
coDynamicArray<T>::coDynamicArray(const coArray<T>& _)
	: coDynamicArray()
{
	operator=(_);
}

template <class T>
coDynamicArray<T>::coDynamicArray(const coDynamicArray<T>& _)
	: coDynamicArray(static_cast<const coArray<T>&>(_))
{

}

template <class T>
coDynamicArray<T>::coDynamicArray(coDynamicArray<T>&& _other)
	: Super(_other)
	, allocator(_other.allocator)
	, capacity(_other.capacity)
{
	_other.capacity = 0;
}

template <class T>
coDynamicArray<T>& coDynamicArray<T>::operator=(const coArray<const T>& _other)
{
	coReserve(*this, _other.count);
	coMemCopy(data, _other.data, _other.count * sizeof(T));
	count = _other.count;
	return *this;
}

template <class T>
coDynamicArray<T>& coDynamicArray<T>::operator=(const coDynamicArray<T>& _other)
{
	return operator=(static_cast<const coArray<const T>&>(_other));
}

/*

template <class T>
void coRemoveUnordered(coDynamicArray<T>& _array, )
{
}
*/
