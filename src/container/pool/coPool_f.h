// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPool.h"
#include "../handle/coHandle.h"
#include "../array/coDynamicArray_f.h"

template <class T, class IndexType>
void coReserve(coPool<T, IndexType>& p, IndexType capacity_)
{
	if (capacity_ > p.capacity)
	{
		const IndexType oldCapacity = p.capacity;
		coReserve(p.buffer, capacity * sizeof(T));
		coReserve(p.freeEntries, capacity);
		for (IndexType i = oldCapacity; i < p.freeEntries.capacity; ++i)
			coPushBack(p.freeEntries, i);
	}
}

template <class T, class IndexType>
T& coGet(coPool<T, IndexType>& p, const coHandle<T, IndexType>& handle)
{
	coASSERT(handle < p.capacity);
	return static_cast<T*>(p.buffer)[handle];
}

template <class T, class IndexType>
coHandle<T, IndexType> coCreate(coPool<T, IndexType>& p)
{
	if (p.nbFreeEntries == 0)
		coReserve(p, p.capacity + 1);
	const IndexType index = p.freeEntries[--p.nbFreeEntries];
	new (static_cast<T*>(p.buffer)[index])();
	return coHandle<T, IndexType>(index);
}

template <class T, class IndexType>
void coDestroy(coPool<T, IndexType>& p, const coHandle<T, IndexType>& handle)
{
	coASSERT(handle < p.capacity);
	static_cast<T*>(p.buffer)[handle]->~T();
	p.freeEntries[p.nbFreeEntries++] = handle;
}
