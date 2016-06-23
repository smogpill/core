// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"

template <class T>
class coConstArray
{
public:
	coConstArray() : data(nullptr), size(0) {}
	coConstArray(const T* _data, coUint32 _size) : data(_data), size(_size) {}
	coFORCE_INLINE const T& operator[] (coUint32 _i) const { coASSERT(_i < size); return data[_i]; }

	const T* data;
	coUint32 size;
};
