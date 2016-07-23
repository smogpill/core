// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coConstArray.h"

class coConstString16 : public coArray<const coChar16>
{
public:
	coConstString16() = default;
	coConstString16(const coChar16* _s);
	coConstString16(const coChar16* _s, coUint32 _size);

	static const coConstString16& GetEmpty();
};
