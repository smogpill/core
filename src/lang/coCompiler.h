// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coConfig
{
public:
	static bool breakOnError;
};

#if defined(_MSC_VER)
#define coMSVC_COMPILER
#	ifdef _DEBUG
#		define coDEBUG
#	endif
#elif defined(__MINGW32__) || defined(__MINGW64__)
#	define coGCC_COMPATIBLE_COMPILER
#	define coMINGW_COMPILER
#elif defined(__GCC__)
#	define coGCC_COMPATIBLE_COMPILER
#	define coGCC_COMPILER
#	ifndef NDEBUG
#		define coDEBUG
#	endif
#elif defined(__clang__)
#	define coCLANG_COMPILER
#else
#	error "Unknown compiler"
#endif

#ifdef coMSVC_COMPILER
#	pragma warning (disable:4577) // warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. 
#endif

inline void _coReturnVoid(int) {}  // to avoid some gcc warnings with the comma operator

#ifdef coMSVC_COMPILER
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && (__debugbreak(), 1))
#else
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && ::raise(SIGINT))
#endif

#if coGCC_COMPATIBLE_COMPILER || coCLANG_COMPILER
#	define coCRASH() (coBREAK(), __builtin_trap())
#else
#	define coCRASH() (coBREAK(), ((void)(*(volatile char*)0 = 0)))
#endif

#ifdef coMSVC_COMPILER
#	define coFORCE_INLINE __forceinline
#else
#	define coFORCE_INLINE inline __attribute__((always_inline))
#endif