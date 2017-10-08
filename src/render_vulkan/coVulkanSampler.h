// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderSampler.h"

class coVulkanSampler : public coRenderSampler
{
	coDECLARE_SUPER(coRenderSampler);
public:
	coVulkanSampler();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkSampler sampler_vk;
};
