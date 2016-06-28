// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/compiler.h"

#ifdef coMSVC_COMPILER
#define coFORCE_INLINE __forceinline
#else
#define coFORCE_INLINE inline __attribute__((always_inline))
#endif

template <class T, class A>
coFORCE_INLINE const T& coBitCast(const A& _a)
{
	static_assert(sizeof(A) == sizeof(T), "");
	return reinterpret_cast<const T&>(_a);
}

template <class T, class A>
coFORCE_INLINE T& coBitCast(A& _a)
{
	static_assert(sizeof(A) == sizeof(T), "");
	return reinterpret_cast<T&>(_a);
}
