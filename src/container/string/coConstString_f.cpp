// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/pch.h"
#include "container/string/coConstString_f.h"

coConstString::coConstString(const coChar* _s)
	: coConstArray(_s, static_cast<coUint32>(::strlen(_s)))
{

}

coConstString::coConstString(const coChar* _s, coUint32 _size)
	: coConstArray(_s, _size)
{

}
