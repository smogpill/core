// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coConstString.h"
#include "container/string/coString.h"

coConstString::coConstString(const coChar* s)
	: coArray(s, s ? static_cast<coUint32>(::strlen(s)) : 0)
{

}

coConstString::coConstString(const coChar* s, coUint32 size)
	: coArray(s, size)
{

}

coConstString::coConstString(const coString& s)
	: coArray(s.data, s.count)
{

}

const coConstString& coConstString::GetEmpty()
{
	static const coConstString a;
	return a;
}
