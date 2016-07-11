// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log/coAssert.h"

template <class T>
class coArray
{
public:
	coArray() : data(nullptr), count(0) {}
	coArray(T* _data, coUint32 _count) : data(_data), count(_count) {}
	coFORCE_INLINE T& operator[] (coUint32 _i) { coASSERT(_i < count); return const_cast<T&>(data[_i]); }
	coFORCE_INLINE const T& operator[] (coUint32 _i) const { coASSERT(_i < count); return data[_i]; }

	T* data;
	coUint32 count;
};
