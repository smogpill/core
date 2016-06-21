// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/DynamicArray.h"

namespace co
{
	class ConstString;
	class String;

	class DynamicString : public DynamicArray<char>
	{
	public:
		DynamicString();
		DynamicString(Allocator& _allocator);
		DynamicString(const ConstString& _s);
		operator const String&() { return reinterpret_cast<String&>(*this); }
		operator const ConstString&() const { return reinterpret_cast<const ConstString&>(*this); }

		void operator=(const ConstString& _s);
	};
}
