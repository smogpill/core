// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "memory/pch.h"
#include "memory/allocator/coStackAllocator.h"

void* coStackAllocator::Allocate(coUint32 _size8)
{
	return coAllocator::GetHeap()->Allocate(_size8);
}

void* coStackAllocator::AllocateAligned(coUint32 _size8, coUint _alignment)
{
	return coAllocator::GetHeap()->AllocateAligned(_size8, _alignment);
}

void coStackAllocator::Free(void* _p)
{
	coAllocator::GetHeap()->Free(_p);
}

void coStackAllocator::FreeAligned(void* _p)
{
	coAllocator::GetHeap()->FreeAligned(_p);
}
