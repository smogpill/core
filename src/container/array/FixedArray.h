// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/Array.h"

namespace co
{
	template <typename T, uint32 CAPACITY>
	class FixedArray : public Array<T>
	{
	public:
		FixedArray();

	private:
		T fixedBuffer[CAPACITY];
	};
}
