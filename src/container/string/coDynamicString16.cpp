// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString16.h"

coDynamicString16::coDynamicString16(coAllocator& _allocator)
	: coDynamicArray<coChar16>(_allocator)
{

}
