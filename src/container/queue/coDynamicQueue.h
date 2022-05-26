// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coQueue.h"
#include "memory/allocator/coAllocator.h"

template <class T>
class coDynamicQueue : public coQueue<T>
{
	typedef coQueue<T> Super;
public:
	~coDynamicQueue()
	{
		coAllocator* allocator = coAllocator::GetHeap();
		coAllocator::GetHeap()->FreeAligned(this->data);
	}
	coUint32 capacity = 0;
};