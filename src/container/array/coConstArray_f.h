// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coConstArray.h"

template <class T>
coConstArray<T>::coConstArray(std::initializer_list<T> _l)
	: data(_l.begin())
	, count(static_cast<coUint32>(_l.size()))
{
}

template <class T> coFORCE_INLINE const T* coBegin(const coConstArray<T>& _a) { return _a.data; }
template <class T> coFORCE_INLINE const T* coEnd(const coConstArray<T>& _a) { return _a.data + _a.count; }

// STD compat
template <class T> coFORCE_INLINE const T* begin(const coConstArray<T> _a) { return coBegin(_a); }
template <class T> coFORCE_INLINE const T* end(const coConstArray<T> _a) { return coEnd(_a); }
