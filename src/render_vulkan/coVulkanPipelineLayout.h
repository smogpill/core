// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderPipelineLayout.h"

class coVulkanDescriptorSetLayout;

class coVulkanPipelineLayout : public coRenderPipelineLayout
{
	coDECLARE_SUPER(coRenderPipelineLayout);
public:
	coVulkanPipelineLayout();
	virtual ~coVulkanPipelineLayout();

	const VkPipelineLayout& GetVkPipelineLayout() const { return pipelineLayout_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkPipelineLayout pipelineLayout_vk;

	// hack
	coVulkanDescriptorSetLayout* vulkandescriptorSetLayout;
};
