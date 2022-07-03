// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "memory/pch.h"
#include "memory/allocator/coLocalAllocator.h"
#include "debug/log/coAssert.h"
#include "debug/log/coLog.h"

coLocalAllocator::coLocalAllocator(coUint64 _maxStackAlloc)
	: maxStackAlloc(_maxStackAlloc)
{
	coASSERT(maxStackAlloc < 16 * 1024); // safety
	coAllocator* stackAllocator = coAllocator::GetStack();
	coASSERT(stackAllocator);
	stackBuffer = static_cast<coByte*>(stackAllocator->Allocate(_maxStackAlloc));
	coASSERT(stackBuffer);

	coTODO("Check for memory leaks");
}

coLocalAllocator::~coLocalAllocator()
{
	coAllocator* stackAllocator = coAllocator::GetStack();
	stackAllocator->Free(stackBuffer);
}

void* coLocalAllocator::Allocate(coUint64 _size8)
{
	const coUint64 newStackAllocated = allocatedStackSize8 + _size8;
	if (newStackAllocated <= maxStackAlloc)
	{
		void* p = stackBuffer + allocatedStackSize8;
		allocatedStackSize8 = newStackAllocated;
		return p;
	}
	else
	{
		coAllocator* defaultAllocator = coAllocator::GetHeap();
		coASSERT(defaultAllocator);
		return defaultAllocator->Allocate(_size8);
	}
}

void* coLocalAllocator::AllocateAligned(coUint64 _size8, coUint _alignment)
{
	const coUint64 newStackAllocated = allocatedStackSize8 + _size8 + _alignment;
	if (newStackAllocated <= maxStackAlloc)
	{
		void* buffer = stackBuffer + allocatedStackSize8;
		void* p = reinterpret_cast<void*>((reinterpret_cast<coIntPtr>(buffer) & ~(coIntPtr(_alignment - 1))) + _alignment);
		//*(reinterpret_cast<void**>(p) - 1) = buffer;
		allocatedStackSize8 = newStackAllocated;
		return p;
	}
	else
	{
		coAllocator* defaultAllocator = coAllocator::GetHeap();
		coASSERT(defaultAllocator);
		return defaultAllocator->AllocateAligned(_size8, _alignment);
	}
}

void coLocalAllocator::Free(void* _p)
{
	if (_p < stackBuffer || _p >= (stackBuffer + maxStackAlloc))
	{
		coAllocator* defaultAllocator = coAllocator::GetHeap();
		coASSERT(defaultAllocator);
		defaultAllocator->Free(_p);
	}
}

void coLocalAllocator::FreeAligned(void* _p)
{
	if (_p < stackBuffer || _p >= (stackBuffer + maxStackAlloc))
	{
		coAllocator* defaultAllocator = coAllocator::GetHeap();
		coASSERT(defaultAllocator);
		defaultAllocator->FreeAligned(_p);
	}
}
