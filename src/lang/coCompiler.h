// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coConfig
{
public:
	static volatile bool breakOnError;
};

#if defined(__clang__)
#	define coCLANG
#elif defined(__MINGW32__) || defined(__MINGW64__)
#	define coGCC_COMPATIBLE
#	define coMINGW
#elif defined(__GNUC__)
#	define coGCC_COMPATIBLE
#	define coGCC
#elif defined(_MSC_VER)
#	define coMSVC
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

#ifdef coMSVC
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && _IsDebuggerPresent() && (__debugbreak(), 1))
#else
#	define coBREAK() _coReturnVoid(coConfig::breakOnError && _IsDebuggerPresent() && ::raise(SIGINT))
#endif

#if defined(coGCC_COMPATIBLE) || defined(coCLANG)
#	define coCRASH() (coBREAK(), __builtin_trap())
#else
#	define coCRASH() (coBREAK(), ((void)(*(volatile char*)0 = 0)))
#endif

#ifdef coMSVC
#	define coFORCE_INLINE __forceinline
#	define coNO_INLINE __declspec(noinline)
#elif defined(coGCC_COMPATIBLE)
#	define coFORCE_INLINE [[gnu::always_inline]]
#	define coNO_INLINE __attribute__((noinline))
#elif defined(coCLANG)
#	define coFORCE_INLINE [[clang::always_inline]]
#	define coNO_INLINE __attribute__((noinline))
#else
# error "Missing compiler impl"
#endif
