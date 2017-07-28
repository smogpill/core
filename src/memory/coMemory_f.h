// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"

void coFillAsDeleted(void* _ptr, coUint _size8);

coFORCE_INLINE coBool coIsAligned(const void* _ptr, coUint _alignment8)
{
	return (coBitCast<coUintPtr>(_ptr) & (_alignment8 - 1)) == 0;
}

coFORCE_INLINE void coMemCopy(void* _dest, const void* _src, coUint _size8)
{
	::memcpy(_dest, _src, _size8);
}
coFORCE_INLINE void coMemMove(void* _dest, const void* _src, coUint _size8)
{
	::memmove(_dest, _src, _size8);
}

template <class T>
void coFill(void* _ptr, coUint _count, const T& _val)
{
	T* p = static_cast<T*>(_ptr);
	for (coUint i = 0; i < _count; ++i)
	{
		*p++ = _val;
	}
}

template <typename T>
coFORCE_INLINE void coSwap(T& _a, T& _b)
{
	// should be faster than other methods because of many possible compiler optimizations
	const T tmp(std::move(_a));
	_a = std::move(_b);
	_b = std::move(tmp);
}

template <typename T>
coFORCE_INLINE void coSwapMemory(T& _a, T& _b)
{
	coByte tmp[sizeof(T)];
	coMemCopy(tmp, &_a, sizeof(_a));
	coMemCopy(&_a, &_b, sizeof(_b));
	coMemCopy(&_b, tmp, sizeof(tmp));
}
