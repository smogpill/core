// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/allocator/coAllocator.h"

class coLocalAllocator : public coAllocator
{
public:
	coLocalAllocator(coUint64 _maxStackAlloc);
	virtual ~coLocalAllocator();

	void* Allocate(coUint64 _size8) override;
	void* AllocateAligned(coUint64 _size8, coUint _alignment) override;
	void Free(void* _p) override;
	void FreeAligned(void* _p) override;

	coByte* stackBuffer = nullptr;
	coUint64 allocatedStackSize8 = 0;
	coUint64 maxStackAlloc = 0;
};
