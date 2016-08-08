// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coSurface.h"

class coVulkanSurface final : public coSurface
{
	coDECLARE_SUPER(coSurface);
public:
	coVulkanSurface();
	~coVulkanSurface();

	const VkSurfaceKHR& GetVkSurfaceKHR() const { return surface_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	VkSurfaceKHR surface_vk;
};
