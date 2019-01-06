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

template <class T>
template <coUint N>
coDynamicArray<T>::coDynamicArray(const T(&_a)[N])
	: coDynamicArray()
{
	coResize(*this, N);
	coMemCopy(data, _a, sizeof(_a));
}

template <class T>
coDynamicArray<T>::coDynamicArray(std::initializer_list<T> _l)
	: coDynamicArray()
{
	coResize(*this, static_cast<coUint32>(_l.size()));
	coMemCopy(data, _l.begin(), static_cast<coUint>(_l.size() * sizeof(T)));
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
		// Aligned to min 16 bytes:
		// - For simplicity. For example for working using SIMD on a float array.
		// - Seems faster on Intel architectures https://software.intel.com/en-us/articles/data-alignment-when-migrating-to-64-bit-intel-architecture).
		const coUint alignment = alignof(T) > 16 ? alignof(T) : 16;
		T* newBuffer = static_cast<T*>(_this.allocator->AllocateAligned(bestCapacity * sizeof(T), alignment));
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
void coInsert(coDynamicArray<T>& _this, coUint32 _pos, const T& _val)
{
	coASSERT(_pos <= _this.count);
	coReserve(_this, _this.count + 1);
	for (coUint i = _this.count; i > _pos; --i)
	{
		_this.data[i] = _this.data[i - 1];
	}
	_this.data[_pos] = _val;
	++_this.count;
}

template <class T>
void coSwap(coDynamicArray<T>& _this, coDynamicArray<T>& _other)
{
	coSwapMemory(_this, _other);
}

template <class T>
coDynamicArray<T>::coDynamicArray(const coArray<T>& _this)
	: coDynamicArray()
{
	operator=(_this);
}

template <class T>
coDynamicArray<T>::coDynamicArray(const coDynamicArray<T>& _this)
	: coDynamicArray(static_cast<const coArray<T>&>(_this))
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
coDynamicArray<T>& coDynamicArray<T>::operator=(const coArray<T>& _other)
{
	return operator=(reinterpret_cast<const coArray<const T>&>(_other));
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
	return operator=(reinterpret_cast<const coArray<const T>&>(_other));
}

// template <class T>
// coDynamicArray<T>& coDynamicArray<T>::operator=(const coDynamicArray<T>& _other)
// {
// 	return operator=(static_cast<const coArray<const T>&>(_other));
// }

/*

template <class T>
void coRemoveUnordered(coDynamicArray<T>& _array, )
{
}
*/
