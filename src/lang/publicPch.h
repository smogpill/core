// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#undef _HAS_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#include <cstddef>
#include <cstdint>
#include <xmmintrin.h>
#include <type_traits>
#include <limits>
#include <new>

#include "lang/types/coBaseTypes.h"
#include "lang/coCompiler.h"
#include "memory/publicPch.h"