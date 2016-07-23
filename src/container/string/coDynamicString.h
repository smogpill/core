// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coConstString;
class coString;

class coDynamicString : public coDynamicArray<coChar>
{
	typedef coDynamicArray<coChar> Super;
public:
	coDynamicString() = default;
	coDynamicString(coAllocator& _allocator);
	coDynamicString(coDynamicString&& _);
	coDynamicString(const coConstString& _s);
	coDynamicString(const coDynamicString& _);
	operator const coString&() { return reinterpret_cast<coString&>(*this); }
	operator const coConstString&() const { return reinterpret_cast<const coConstString&>(*this); }

	coDynamicString& operator=(const coDynamicString& _s);
	coDynamicString& operator=(const coConstString& _s);
};
