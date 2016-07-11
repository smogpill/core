// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coAllocator;

template <class T>
class coDynamicArray : public coArray<T>
{
public:
	coDynamicArray();
	coDynamicArray(coAllocator& _allocator);
	~coDynamicArray();

	coUint32 capacity;
	coAllocator* allocator;

protected:
	coDynamicArray<T>(const coDynamicArray<T>&) = delete;
	coDynamicArray<T>& operator=(const coDynamicArray<T>&) = delete;
};
