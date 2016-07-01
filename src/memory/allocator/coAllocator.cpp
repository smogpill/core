// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "memory/pch.h"
#include "memory/allocator/coAllocator.h"
#include "memory/allocator/coHeapAllocator.h"

coAllocator * coAllocator::GetHeap()
{
	if (!initialized)
	{
		Init();
	}
	return heap;
}

void coAllocator::Init()
{
	if (initialized)
		return;
	static coHeapAllocator defaultHeapAllocator;
	coAllocator::heap = &defaultHeapAllocator;
	coAllocator::frame = nullptr;
}

coAllocator* coAllocator::heap = nullptr;
coAllocator* coAllocator::frame = nullptr;
bool coAllocator::initialized = false;
