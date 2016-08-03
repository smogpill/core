// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coRendererInfo_vk.h"

coRendererInfo_vk::coRendererInfo_vk()
	: instance_vk(VK_NULL_HANDLE)
	, enableDebug(false)
	, messageHandler_vk(nullptr)
{
#ifdef coDEBUG
	enableDebug = true;
#endif
}
