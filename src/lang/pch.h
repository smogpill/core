// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <initializer_list>
#include "lang/publicPCH.h"
#ifdef _WIN32
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX

//#	define _HAS_EXCEPTIONS 0 // to avoid warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. 

// [BUILD-OPT] Exclude rarely-used stuff from Windows headers
// VC_EXTRALEAN defines WIN32_LEAN_AND_MEAN and a number of NOservice definitions, such as NOCOMM and NOSOUND
// See afxv_w32.h
#	define VC_EXTRALEAN
#	define NOLANGUAGE
#	define NOKEYBOARDINFO
#	define NOLSTRING
#	include <windows.h>
#endif
