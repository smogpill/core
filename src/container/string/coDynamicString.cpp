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

coDynamicString::coDynamicString(const coDynamicString& _)
	: coDynamicString(static_cast<const coConstString&>(_))
{

}

coDynamicString::coDynamicString(coAllocator& _allocator)
	: coDynamicArray<coChar>(_allocator)
{

}

coDynamicString::coDynamicString(coDynamicString&& _)
	: Super(_)
{

}

coDynamicString& coDynamicString::operator=(const coDynamicString& _s)
{
	operator=(static_cast<const coConstString&>(_s));
	return *this;
}

coDynamicString& coDynamicString::operator=(const coConstString& _s)
{
	Super::operator=(_s);
	return *this;
}
