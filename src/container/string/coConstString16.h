// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coConstArray.h"

class coConstString16 : public coArray<const coWideChar>
{
	typedef coArray<const coWideChar> Super;
public:
	coConstString16() = default;
	coConstString16(const coWideChar* _s);
	coConstString16(const coWideChar* _s, coUint32 _size);
	coConstString16(const Super& _);

	static const coConstString16& GetEmpty();
};
