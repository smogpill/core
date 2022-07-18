// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coArray.h"
#include "memory/allocator/coAllocator.h"

template <class T>
class coReverseDynamicArray : public coArray<T>
{
	typedef coArray<T> Super;
public:
	coReverseDynamicArray() = default;
	~coReverseDynamicArray()
	{
		coAllocator* allocator = coAllocator::GetHeap();
		coAllocator::GetHeap()->FreeAligned(this->data);
	}

	coUint32 capacity = 0;
};
