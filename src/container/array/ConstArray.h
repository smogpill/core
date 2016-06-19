// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/types/baseTypes.h"
#include "lang/cppExtensions.h"

namespace co
{
	template <class T>
	class ConstArray
	{
	public:
		ConstArray() : data(nullptr), size(0) {}
		ConstArray(const T* _data, uint32 _size) : data(_data), size(_size) {}
		coFORCE_INLINE const T& operator[] (uint32 _i) const { coASSERT(_i < size); return data[_i]; }

		const T* data;
		uint32 size;
	};
}
