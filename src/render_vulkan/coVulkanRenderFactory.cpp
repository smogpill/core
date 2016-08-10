// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanRenderer.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render/coRenderFactory.h"

coRenderer* coCreateRenderer()
{
	return new coVulkanRenderer();
}

coSwapChain* coCreateSwapChain()
{
	return new coVulkanSwapChain();
}

coSurface* coCreateSurface()
{
	return new coVulkanSurface();
}
