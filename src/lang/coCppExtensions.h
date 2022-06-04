// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCompiler.h"

coFORCE_INLINE constexpr bool coReturnFalse() { return false; }
coFORCE_INLINE constexpr bool coReturnTrue() { return true; }

#define coSAFE_SCOPE(_x_) do { _x_ } while (coReturnFalse())

#ifdef coDEBUG
#	define coDEBUG_CODE(_x_) _x_
#else
#	define coDEBUG_CODE(_x_)
#endif

template <class T, class A>
coFORCE_INLINE constexpr const T& coBitCast(const A& _a)
{
	//static_assert(sizeof(A) == sizeof(T), "");
	return reinterpret_cast<const T&>(_a);
}

template <class T, class A>
coFORCE_INLINE constexpr T& coBitCast(A& _a)
{
	//static_assert(sizeof(A) == sizeof(T), "");
	return reinterpret_cast<T&>(_a);
}

#define _coSTRINGIFY(_x_) #_x_
#define coSTRINGIFY(_x_) _coSTRINGIFY(_x_)
#define _coCONCAT(_x_, _y_) _x_ ## _y_
#define coCONCAT(_x_, _y_) _coCONCAT(_x_, _y_)


#ifdef coMSVC_COMPILER
#	define coPRAGMA_MESSAGE(_x_)	__pragma(message(__FILE__ "("  coSTRINGIFY(__LINE__) "): " _x_))
#	define coNOEXCEPT				throw()
#else
#	define coPRAGMA_MESSAGE(_x_)	_Pragma(_coSTRINGIFY(message "Warning: " _x_))
#	define coNOEXCEPT				noexcept
#endif

#ifdef coCLANG_COMPILER
#	define coMETA(...) [[__VA_ARGS__]] //__attribute__((annotate(#__VA_ARGS__)))
#	define coNO_INIT [[gsl::suppress("type.6")]]
#else
#	define coMETA(...)
#	define coNO_INIT [[gsl::suppress(type.6)]]
#endif

#ifdef coMSVC_COMPILER
#	define coFORCE_SYMBOL_INCLUSION_ATTRIBUTE __declspec(dllexport)
#else
#	define coFORCE_SYMBOL_INCLUSION_ATTRIBUTE __attribute__ ((used))
#endif

#ifdef coMSVC_COMPILER
#	define coLIKELY(_x_) (_x_)
#	define coUNLIKELY(_x_) (_x_)
#else
#	define coLIKELY(_x_) __builtin_expect(!!(_x_), true)
#	define coUNLIKELY(_x_) __builtin_expect(!!(_x_), false)
#endif

#ifdef coMSVC_COMPILER
#	define coRESTRICT __restrict
#else
#	define coRESTRICT __restrict__
#endif