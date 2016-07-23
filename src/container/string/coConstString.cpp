// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coConstString.h"

coConstString::coConstString(const coChar* _s)
	: coArray(_s, _s ? static_cast<coUint32>(::strlen(_s)) : 0)
{

}

coConstString::coConstString(const coChar* _s, coUint32 _size)
	: coArray(_s, _size)
{

}

const coConstString& coConstString::GetEmpty()
{
	static const coConstString a;
	return a;
}
