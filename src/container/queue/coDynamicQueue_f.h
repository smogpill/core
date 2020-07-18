// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coDynamicQueue.h"
#include "coQueue_f.h"
#include "../array/coDynamicArray_f.h"
#include <math/scalar/coUint32_f.h>

template <class T>
void coReserve(coDynamicQueue<T>& q, coUint32 nbDesired)
{
	if (nbDesired > q.capacity)
	{
		const coUint32 bestCapacity = _coComputeBestArrayCapacity(nbDesired);
		// Aligned to min 16 bytes:
		// - For simplicity. For example for working using SIMD on a float array.
		// - Seems faster on Intel architectures https://software.intel.com/en-us/articles/data-alignment-when-migrating-to-64-bit-intel-architecture).
		const coUint alignment = alignof(T) > 16 ? alignof(T) : 16;
		T* newBuffer = static_cast<T*>(coAllocator::GetHeap()->AllocateAligned(bestCapacity * sizeof(T), alignment));
		if (q.data)
		{
			const coUint32 end = q.begin + q.count;
			if (end <= q.capacity)
			{
				coMemCopy(newBuffer, &q.data[q.begin], q.count * sizeof(T));
			}
			else
			{
				const coUint32 nbFirst = q.capacity - q.begin;
				coMemCopy(newBuffer, &q.data[q.begin], nbFirst * sizeof(T));
				coASSERT(q.count - nbFirst);
				coMemCopy(&newBuffer[nbFirst], q.data, (q.count - nbFirst) * sizeof(T));
			}
			q.begin = 0;
			coAllocator::GetHeap()->FreeAligned(q.data);
		}
		q.data = newBuffer;
		q.capacity = bestCapacity;
		coASSERT(coIsPowerOf2(q.capacity));
	}
}

template <class T>
T& coPushBack(coDynamicQueue<T>& q)
{
	const coUint32 nextCount = q.count + 1;
	if (q.count == q.capacity)
		coReserve(q, nextCount);
	const coUint32 index = (q.begin + q.count) & (q.capacity - 1);
	q.count = nextCount;
	return q.data[index];
}

template <class T>
void coPushBack(coDynamicQueue<T>& q, const T& v)
{
	coASSERT(!coContainsPointer(coArray<T>(q.data, q.capacity), &v));
	coPushBack(q) = v;
}

template <class T>
const T& coPopFront(coDynamicQueue<T>& q)
{
	coASSERT(!coIsEmpty(q));
	const coUint32 index = q.begin;
	q.begin = (q.begin + 1) & (q.capacity - 1);
	--q.count;
	return q.data[index];
}
