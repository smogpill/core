// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/ConstArray.h"

namespace co
{
	class ConstString : public ConstArray<char>
	{
	public:
		ConstString();
		ConstString(const char* _s, uint32 _size);
	};
}
