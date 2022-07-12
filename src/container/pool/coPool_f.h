// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPool.h"
#include "../handle/coHandle.h"
#include "../array/coDynamicArray_f.h"

template <class T, class IndexType>
void coReserve(coPool<T, IndexType>& p, IndexType capacity)
{
	coReserve(p.elements, capacity);
	coReserve(p.freeEntries, capacity);
}

template <class T, class IndexType>
coHandle<T> coCreate(coPool<T, IndexType>& p)
{
	if (p.freeEntries.count == 0)
		coReserve(p, p.freeEntries.capacity + 64);
	const IndexType index = coPopBack(p.freeEntries);
	return coHandle<T>(index);
}

template <class T, class IndexType>
void coDestroy(coPool<T, IndexType>& p, const coHandle<T>& handle)
{
	coPushBack(p.freeEntries, handle);
}