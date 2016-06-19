// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/DynamicArray.h"

namespace co
{
	class Allocator;

	template <class T>
	class DynamicArray : public Array<T>
	{
	public:
		DynamicArray();
		DynamicArray(Allocator& _allocator);
		~DynamicArray();

		uint32 capacity;
		Allocator* allocator;
	};
}
