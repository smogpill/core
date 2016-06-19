// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

namespace co
{
	struct config
	{
		static bool breakOnError;
	};

#if defined(_MSC_VER)
#define coMSVC_COMPILER
#	ifdef _DEBUG
#		define coDEBUG
#	endif
#elif defined(__MINGW32__) || defined(__MINGW64__)
#define coGCC_COMPATIBLE_COMPILER
#define coMINGW_COMPILER
#elif defined(__GCC__)
#define coGCC_COMPATIBLE_COMPILER
#define coGCC_COMPILER
#else
#error "Unknown compiler"
#endif

	inline void _coReturnVoid(int) {}  // to avoid some gcc warnings with the comma operator
#ifdef coMSVC_COMPILER
#define coBREAK() _coReturnVoid(config::breakOnError && (__debugbreak(), true))
#else
#define coBREAK() _coReturnVoid(config::breakOnError && ::raise(SIGINT))
#endif

}
