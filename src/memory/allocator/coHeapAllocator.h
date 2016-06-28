// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/allocator/coAllocator.h"

class coHeapAllocator : public coAllocator
{
public:
	virtual ~coHeapAllocator() {}

	virtual void* allocate(coUint32 _size8) override;
	virtual void* allocateAligned(coUint32 _size8, coUint _alignment) override;
	virtual void free(void* _p) override;
	virtual void freeAligned(void* _p) override;
};
