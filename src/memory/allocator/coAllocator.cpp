// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "memory/pch.h"
#include "memory/allocator/coAllocator.h"
#include "memory/allocator/coHeapAllocator.h"
#include "memory/allocator/coStackAllocator.h"

coAllocator* coAllocator::heap = nullptr;
coAllocator* coAllocator::frame = nullptr;
coAllocator* coAllocator::debug = nullptr;
coAllocator* coAllocator::stack = nullptr;
bool coAllocator::initialized = false;

coAllocator* coAllocator::GetHeap()
{
	if (!initialized)
	{
		Init();
	}
	return heap;
}

coAllocator* coAllocator::GetDebug()
{
	if (!initialized)
	{
		Init();
	}
	return debug;
}

coAllocator* coAllocator::GetFrame()
{
	if (!initialized)
	{
		Init();
	}
	return frame;
}

coAllocator* coAllocator::GetStack()
{
	if (!initialized)
	{
		Init();
	}
	return stack;
}

void coAllocator::Init()
{
	if (initialized)
		return;
	static coHeapAllocator defaultHeapAllocator;
	static coStackAllocator defaultStackAllocator;
	coAllocator::heap = &defaultHeapAllocator;
	coAllocator::frame = nullptr;
	coAllocator::stack = &defaultStackAllocator;
	coAllocator::debug = &defaultHeapAllocator;
}

