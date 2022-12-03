// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"

#define STB_IMAGE_IMPLEMENTATION
#ifdef coMSVC_COMPILER
	#pragma warning(push)
	#pragma warning(disable:4505) // warning C4505: 'stbi__cpuid3': unreferenced function with internal linkage has been removed
#endif
#include <stb/stb_image.h>
#ifdef coMSVC_COMPILER
	#pragma warning(pop)
#endif