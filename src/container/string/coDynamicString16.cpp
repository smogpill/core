// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coConstString16.h"
#include "container/array/coDynamicArray_f.h"

coDynamicString16::coDynamicString16(const coConstString16& _s)
{
	operator=(_s);
}

coDynamicString16& coDynamicString16::operator=(const coConstString16& _s)
{
	Super::operator=(_s);
	return *this;
}
