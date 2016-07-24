// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "memory/pch.h"
#include "memory/allocator/coLocalAllocator.h"
#include "debug/log/coAssert.h"
#include "debug/log/coLog.h"

coLocalAllocator::coLocalAllocator(coUint32 _maxStackAlloc)
	: stackBuffer(nullptr)
	, stackAllocated(0)
	, maxStackAlloc(_maxStackAlloc)
{
	coASSERT(maxStackAlloc < 16 * 1024); // safety
	stackBuffer = static_cast<coByte*>(::alloca(maxStackAlloc));
	coASSERT(stackBuffer);

	coTODO("Check memory leaks");
}

coLocalAllocator::~coLocalAllocator()
{
}

void* coLocalAllocator::Allocate(coUint32 _size8)
{
	const coUint32 newStackAllocated = stackAllocated + _size8;
	if (newStackAllocated <= maxStackAlloc)
	{
		void* p = stackBuffer + stackAllocated;
		stackAllocated = newStackAllocated;
		return p;
	}
	else
	{
		coAllocator* defaultAllocator = coAllocator::GetHeap();
		coASSERT(defaultAllocator);
		return defaultAllocator->Allocate(_size8);
	}
}

void* coLocalAllocator::AllocateAligned(coUint32 /*_size8*/, coUint /*_alignment*/)
{
	coWARN_NOT_AVAILABLE();
	return nullptr;
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
