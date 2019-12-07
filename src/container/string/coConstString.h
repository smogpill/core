// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coString;

class coConstString : public coArray<const coChar>
{
public:
	coConstString() = default;
	coConstString(const coString& s);
	coConstString(const coChar* s);
	coConstString(const coChar* s, coUint32 size);
	template <coUint N>
	coConstString(const coChar(&s)[N])
		: coConstString(s)
	{

	}

	static const coConstString& GetEmpty();
};
