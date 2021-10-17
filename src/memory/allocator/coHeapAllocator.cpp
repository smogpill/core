// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "memory/pch.h"
#include "memory/allocator/coHeapAllocator.h"

//-------------------------------------------------------
void* coHeapAllocator::Allocate(coUint32 _size8)
{
	return ::malloc(_size8);
}

//-------------------------------------------------------
void* coHeapAllocator::AllocateAligned(coUint32 _size8, coUint _alignment)
{
#ifdef coMSVC_COMPILER
	// Visual Studio does not support C++17's std::aligned_alloc(): https://stackoverflow.com/questions/62962839/stdaligned-alloc-missing-from-visual-studio-2019c
	return ::_aligned_malloc(_size8, _alignment);
#else
	return ::aligned_alloc(_alignment, _size8);
#endif
}

//-------------------------------------------------------
void coHeapAllocator::Free(void* _p)
{
	::free(_p);
}

//-------------------------------------------------------
void coHeapAllocator::FreeAligned(void* _p)
{
#ifdef coMSVC_COMPILER
	::_aligned_free(_p);
#else
	::free(_p);
#endif
}
