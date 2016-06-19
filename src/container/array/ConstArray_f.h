// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/ConstArray.h"

namespace co
{
	template <class T>
	const T* begin(const ConstArray<T>& _a) { return _a.data; }
	template <class T>
	const T* end(const ConstArray<T>& _a) { return _a.data + _a.size; }
}
