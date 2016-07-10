// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coConstString16.h"

coConstString16::coConstString16(const coChar16* _s)
	: coConstArray(_s, _s ? static_cast<coUint32>(::wcslen(_s)) : 0)
{

}

coConstString16::coConstString16(const coChar16* _s, coUint32 _size)
	: coConstArray(_s, _size)
{

}

const coConstString16& coConstString16::GetEmpty()
{
	static const coConstString16 a;
	return a;
}
