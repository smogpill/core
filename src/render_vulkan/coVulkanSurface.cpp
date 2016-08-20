// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanSurface.h"
#include "render_vulkan/coVulkanContext.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "render/coRenderContext.h"
#include "lang/result/coResult_f.h"

coVulkanSurface::coVulkanSurface()
	: surface_vk(VK_NULL_HANDLE)
{

}

coVulkanSurface::~coVulkanSurface()
{
	if (surface_vk != VK_NULL_HANDLE)
	{
		coVulkanContext* vulkanRenderer = static_cast<coVulkanContext*>(renderContext);
		coASSERT(vulkanRenderer);
		const VkInstance& instance_vk = vulkanRenderer->GetVkInstance();
		coASSERT(instance_vk != VK_NULL_HANDLE);
		vkDestroySurfaceKHR(instance_vk, surface_vk, nullptr);
	}
}

coResult coVulkanSurface::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coVulkanContext* vulkanRenderer = static_cast<coVulkanContext*>(renderContext);
	coTRY(vulkanRenderer, nullptr);
	const VkInstance& instance_vk = vulkanRenderer->GetVkInstance();

#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = config.hwnd;
	createInfo.hinstance = ::GetModuleHandle(nullptr);

	auto CreateWin32SurfaceKHR_vk = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(instance_vk, "vkCreateWin32SurfaceKHR"));
	coTRY(CreateWin32SurfaceKHR_vk, "Failed to fetch vkCreateWin32SurfaceKHR");
	coASSERT(surface_vk == nullptr);
	coVULKAN_TRY(CreateWin32SurfaceKHR_vk(instance_vk, &createInfo, nullptr, &surface_vk), "Failed to create Win32 surface.");
#else
#	error Not yet supported
#endif

	return true;
}
