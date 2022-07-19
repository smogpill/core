// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/dynamic/coReverseDynamicArray.h"
#include "../coDynamicArray_f.h"

template <class T>
void _coSetCapacity(coReverseDynamicArray<T>& a, coUint32 capacity)
{
	if (a.capacity != capacity)
	{
		coASSERT(capacity >= a.count);

		// Aligned to min 16 bytes:
		// - For simplicity. For example for working using SIMD on a float array.
		// - Seems faster on Intel architectures https://software.intel.com/en-us/articles/data-alignment-when-migrating-to-64-bit-intel-architecture).
		const coUint alignment = alignof(T) > 16 ? alignof(T) : 16;
		T* newData = static_cast<T*>(coAllocator::GetHeap()->AllocateAligned(coUint64(capacity) * sizeof(T), alignment));
		if (a.data)
		{
			coMemCopy(newData + capacity - a.count, a.data + a.capacity - a.count, coUint64(a.count) * sizeof(T));
			coAllocator::GetHeap()->FreeAligned(a.data);
		}
		a.data = newData;
		a.capacity = capacity;
	}
}

template <class T>
void coReserve(coReverseDynamicArray<T>& a, coUint32 desiredCount)
{
	if (desiredCount > a.capacity)
		_coSetCapacity(a, _coComputeBestArrayCapacity(desiredCount));
}

template <class T>
void coPushBackArray(coReverseDynamicArray<T>& a, const coArray<const T>& other)
{
	//static_assert(std::is_base_of<coArray<T>, A>::value, "_this should be an array");
	coASSERT(~decltype(a.count)(0) - a.count >= other.count);
	const coUint32 desiredCount = a.count + other.count;
	if (desiredCount > a.capacity)
		coReserve(a, desiredCount);

	coMemCopy(&a.data[a.capacity - desiredCount], _other.data, coUint64(other.count) * sizeof(T));
	_this.count = desiredCount;
}

template <class T>
void coPushBackArray(coDynamicArray<T>& _this, const coArray<T>& _other)
{
	coPushBackArray(_this, static_cast<const coArray<const T>&>(_other));
}

template <class T>
void coClear(coReverseDynamicArray<T>& a)
{
	a.count = 0;
}