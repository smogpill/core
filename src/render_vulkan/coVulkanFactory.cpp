// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanContext.h"
#include "render_vulkan/coVulkanSwapChain.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanSemaphore.h"
#include "render/coRenderFactory.h"

coRenderContext* coCreateRenderContext()
{
	return new coVulkanContext();
}

coSwapChain* coCreateSwapChain()
{
	return new coVulkanSwapChain();
}

coSurface* coCreateSurface()
{
	return new coVulkanSurface();
}

coRenderSemaphore* coCreateRenderSemaphore()
{
	return new coVulkanSemaphore();
}
