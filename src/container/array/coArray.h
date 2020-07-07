// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "debug/log/coAssert.h"
class coBinaryOutputStream;

template <class T>
class coArray
{
public:
	typedef T ValueType;
	coArray() = default;
	coArray(T* _data, coUint32 _count) : data(_data), count(_count) {}
	template <coUint N>
	coArray(const T (&_a)[N]);
	coFORCE_INLINE T& operator[] (coUint32 _i) { coASSERT(_i < count); return const_cast<T&>(data[_i]); }
	coFORCE_INLINE const T& operator[] (coUint32 _i) const { coASSERT(_i < count); return data[_i]; }
	//coArray(std::initializer_list<T> _l);
	operator const coArray<const T>& () const { return reinterpret_cast<const coArray<const T>&>(*this); }
	void Write(coBinaryOutputStream& stream) const;

	T* data = nullptr;
	coUint32 count = 0;
};
