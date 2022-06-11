// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#ifdef coRELEASE
#define USE_OPTICK 0
#else
#define USE_OPTICK 1
#endif
#define OPTICK_ENABLE_GPU 0
#include "optick.h"
