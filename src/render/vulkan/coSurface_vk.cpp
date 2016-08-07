// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coSurface_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coRenderContext_vk.h"
#include "lang/result/coResult_f.h"

coSurface_vk::coSurface_vk()
	: renderContext_vk(nullptr)
	, surface_vk(VK_NULL_HANDLE)
{

}

coSurface_vk::~coSurface_vk()
{
	if (surface_vk != VK_NULL_HANDLE)
	{
		coASSERT(renderContext_vk);
		const VkInstance& instance_vk = renderContext_vk->GetVkInstance();
		coASSERT(instance_vk != VK_NULL_HANDLE);
		vkDestroySurfaceKHR(instance_vk, surface_vk, nullptr);
	}
}

coSurface_vk::InitConfig::InitConfig()
	: hwnd(NULL)
	, renderContext_vk(nullptr)
{

}

coResult coSurface_vk::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	renderContext_vk = config.renderContext_vk;
	coTRY(renderContext_vk, nullptr);
	const VkInstance& instance_vk = renderContext_vk->GetVkInstance();

#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = config.hwnd;
	createInfo.hinstance = ::GetModuleHandle(nullptr);

	auto CreateWin32SurfaceKHR_vk = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(vkGetInstanceProcAddr(instance_vk, "vkCreateWin32SurfaceKHR"));
	coTRY(CreateWin32SurfaceKHR_vk, "Failed to fetch vkCreateWin32SurfaceKHR");
	coASSERT(surface_vk == nullptr);
	coTRY_vk(CreateWin32SurfaceKHR_vk(instance_vk, &createInfo, nullptr, &surface_vk), "Failed to create Win32 surface.");
#else
#	error Not yet supported
#endif
	
	return true;
}
