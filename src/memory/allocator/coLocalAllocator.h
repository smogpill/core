// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/allocator/coAllocator.h"

class coLocalAllocator : public coAllocator
{
public:
	coLocalAllocator(coUint32 _maxStackAlloc);
	virtual ~coLocalAllocator();

	virtual void* Allocate(coUint32 _size8) override;
	virtual void* AllocateAligned(coUint32 _size8, coUint _alignment) override;
	virtual void Free(void* _p) override;
	virtual void FreeAligned(void* _p) override;

	coByte* stackBuffer;
	coUint32 allocatedStackSize8;
	coUint32 maxStackAlloc;
};
