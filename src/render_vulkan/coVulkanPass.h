// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderPass.h"

class coVulkanPass : public coRenderPass
{
	coDECLARE_SUPER(coRenderPass);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		VkAttachmentDescription attachmentDescription_vk;
	};
	coVulkanPass();
	virtual ~coVulkanPass();

	const VkRenderPass& GetVkRenderPass() const { return renderPass_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkRenderPass renderPass_vk;
};
