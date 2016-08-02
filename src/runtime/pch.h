// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "runtime/publicPch.h"

#ifdef coMSWINDOWS
#	ifndef NOMINMAX
#	define NOMINMAX
#	endif // NOMINMAX

// [BUILD-OPT] Exclude rarely-used stuff from Windows headers
// VC_EXTRALEAN defines WIN32_LEAN_AND_MEAN and a number of NOservice definitions, such as NOCOMM and NOSOUND
// See afxv_w32.h
#	define VC_EXTRALEAN
#	define NOLANGUAGE
#	define NOKEYBOARDINFO
#	define NOLSTRING
#	include <windows.h>
#endif
