// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderFrameBuffer.h"

class coVulkanFramebuffer : public coRenderFramebuffer
{
	coDECLARE_SUPER(coRenderFramebuffer);
public:
	coVulkanFramebuffer();
	virtual ~coVulkanFramebuffer();

	const VkFramebuffer& GetVkFramebuffer() const { return framebuffer_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkFramebuffer framebuffer_vk;
};
