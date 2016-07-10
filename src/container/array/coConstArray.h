// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "debug/log/coAssert.h"

template <class T>
class coConstArray
{
public:
	coConstArray() : data(nullptr), count(0) {}
	coConstArray(const T* _data, coUint32 _count) : data(_data), count(_count) {}
	coConstArray(std::initializer_list<T> _l);
	coFORCE_INLINE const T& operator[] (coUint32 _i) const { coASSERT(_i < count); return data[_i]; }

	const T* data;
	coUint32 count;
};
