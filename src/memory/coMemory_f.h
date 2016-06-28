// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"

void coAssignAsDeleted(void* _ptr, coUint _size8);

coFORCE_INLINE coBool coIsAligned(const void* _ptr, coUint _alignment8)
{
	return (coBitCast<coUintPtr>(_ptr) & (_alignment8 - 1)) == 0;
}

coFORCE_INLINE void coMemCopy(void* _dest, const void* _src, coUint _size8)
{
#ifdef __STDC_SECURE_LIB__
	memcpy_s(_dest, _size8, _src, _size8);
#else
	memcpy(_dest, _src, _size8);
#endif
}
coFORCE_INLINE void coMemMove(void* _dest, const void* _src, coUint _size8)
{
#ifdef __STDC_SECURE_LIB__
	memmove_s(_dest, _size8, _src, _size8);
#else
	memmove(_dest, _src, _size8);
#endif
}

template <class T>
void coAssign(void* _ptr, coUint _count, const T& _val)
{
	T* p = static_cast<T*>(_ptr);
	for (coUint i = 0; i < _count; ++i)
	{
		*p++ = _val;
	}
}
