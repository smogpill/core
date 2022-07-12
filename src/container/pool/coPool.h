// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../array/coDynamicArray.h"
#include "../handle/coHandle.h"

template <class T, class IndexType>
class coPool
{
public:
	coDynamicArray<T> elements;
	coDynamicArray<IndexType> freeEntries;
};
