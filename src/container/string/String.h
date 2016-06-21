// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/Array.h"

namespace co
{
	class String : public Array<char>
	{
	public:
		String();
		String(char* _s, uint32 _size);
	};
}
