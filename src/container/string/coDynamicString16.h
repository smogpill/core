// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coConstString16;

class coDynamicString16 : public coDynamicArray<coWideChar>
{
	typedef coDynamicArray<coWideChar> Super;
public:
	coDynamicString16() = default;
	coDynamicString16(coAllocator& _allocator);
	coDynamicString16(const coConstString16& _other);

	coDynamicString16& operator=(const coConstString16& _s);
};
