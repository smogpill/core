// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render_vulkan/publicPch.h"
#include "platform/coOsHeaders.h"
#ifdef coMSWINDOWS
#	define VK_USE_PLATFORM_WIN32_KHR
#else
#	error Platform not supported
#endif
#include <vulkan/vulkan.h>

