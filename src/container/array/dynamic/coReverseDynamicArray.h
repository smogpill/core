// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "memory/allocator/coAllocator.h"

template <class T>
class coReverseDynamicArray
{
public:
	coReverseDynamicArray() = default;
	~coReverseDynamicArray()
	{
		coAllocator* allocator = coAllocator::GetHeap();
		coAllocator::GetHeap()->FreeAligned(this->data);
	}

	T* data = nullptr;
	coUint32 count = 0;
	coUint32 capacity = 0;
};
