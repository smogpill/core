// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coDynamicString16 : public coDynamicArray<coChar16>
{
public:
	coDynamicString16() = default;
	coDynamicString16(coAllocator& _allocator);
};
