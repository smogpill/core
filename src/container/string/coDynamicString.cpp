// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString.h"
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray_f.h"

coDynamicString::coDynamicString(const coConstString& _s)
	: coDynamicString()
{
	operator=(_s);
}

coDynamicString::coDynamicString(coAllocator& _allocator)
	: coDynamicArray<coChar>(_allocator)
{

}

coDynamicString& coDynamicString::operator=(const coConstString& _s)
{
	coReserve(*this, _s.count);
	coMemCopy(data, _s.data, _s.count);
	count = _s.count;
	return *this;
}
