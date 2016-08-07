// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderContext_vk;

class coSurface_vk : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
#ifdef VK_USE_PLATFORM_WIN32_KHR
		HWND hwnd;
#endif
		coRenderContext_vk* renderContext_vk;
	};

	coSurface_vk();
	virtual ~coSurface_vk();

	const VkSurfaceKHR& GetVkSurfaceKHR() const { return surface_vk; }
	
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coRenderContext_vk* renderContext_vk;
	VkSurfaceKHR surface_vk;
};
