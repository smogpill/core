// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coConstArray.h"

class coConstString : public coConstArray<coChar>
{
public:
	coConstString() = default;
	explicit coConstString(const coChar* _s);
	coConstString(const coChar* _s, coUint32 _size);
};
