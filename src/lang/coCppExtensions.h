// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCompiler.h"

coFORCE_INLINE bool coReturnFalse() { return false; }
coFORCE_INLINE bool coReturnTrue() { return true; }

#define coSAFE_SCOPE(_x_) do { _x_ } while (coReturnFalse())

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

#define _coSTRINGIFY(_x_) #_x_
#define coSTRINGIFY(_x_) _coSTRINGIFY(_x_)

#ifdef coMSVC_COMPILER
#define coPRAGMA_MESSAGE(_x_)	__pragma(message(__FILE__ "("  coSTRINGIFY(__LINE__) "): " _x_))
#define coNOEXCEPT				throw()
#else
#define coPRAGMA_MESSAGE(_x_)	_Pragma(_coSTRINGIFY(message "Warning: " _x_))
#define coNOEXCEPT				noexcept
#endif
