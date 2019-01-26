// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coBitArray.h"

class coAllocator;

class coDynamicBitArray : public coBitArray
{
	typedef coBitArray Super;
public:
	coDynamicBitArray() {}
	explicit coDynamicBitArray(coAllocator& _allocator);
	~coDynamicBitArray();

	coUint32 capacity = 0;
	coAllocator* allocator = nullptr;
};
