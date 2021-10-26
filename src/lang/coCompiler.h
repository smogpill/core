// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coConfig
{
public:
	static volatile bool breakOnError;
};

#if defined(__clang__)
#	define coCLANG_COMPILER
#elif defined(__MINGW32__) || defined(__MINGW64__)
#	define coGCC_COMPATIBLE_COMPILER
#	define coMINGW_COMPILER
#elif defined(__GNUC__)
#	define coGCC_COMPATIBLE_COMPILER
#	define coGCC_COMPILER
#elif defined(_MSC_VER)
#	define coMSVC_COMPILER
#	pragma warning(disable:4577) // warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. 
#	pragma warning(disable:4324) // warning C4324: structure was padded due to alignment specifier.
#	pragma warning(disable:4718) // warning C4718: recursive call has no side effects, deleting
#	pragma warning(disable:4189) // warning C4189: local variable is initialized but not referenced
#	pragma warning(disable:4100) // warning C4100: unreferenced formal parameter
#else
#	error "Unknown compiler"
#endif

inline void _coReturnVoid(int) {}  // to avoid some gcc warnings with the comma operator
bool _IsDebuggerPresent();

#ifdef coMSVC_COMPILER
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && _IsDebuggerPresent() && (__debugbreak(), 1))
#else
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && _IsDebuggerPresent() && ::raise(SIGINT))
#endif

#if defined(coGCC_COMPATIBLE_COMPILER) || defined(coCLANG_COMPILER)
#	define coCRASH() (coBREAK(), __builtin_trap())
#else
#	define coCRASH() (coBREAK(), ((void)(*(volatile char*)0 = 0)))
#endif

#ifdef coMSVC_COMPILER
#	define coFORCE_INLINE __forceinline
#else
#	define coFORCE_INLINE inline __attribute__((always_inline))
#endif