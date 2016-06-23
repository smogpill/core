// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coConstArray.h"

template <class T>
class coArray : public coConstArray<T>
{
public:
	coArray() {}
	coArray(T* _data, coUint32 _size) : coConstArray(_data, _size) {}
	coFORCE_INLINE T& operator[] (coUint _i) { coASSERT(_i < size); return const_cast<T&>(data[_i]); }
	coFORCE_INLINE const T& operator[] (coUint _i) const { coASSERT(_i < size); return data[_i]; }
};

