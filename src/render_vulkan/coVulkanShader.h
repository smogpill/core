// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coShader.h"

class coVulkanShader : public coShader
{
	coDECLARE_SUPER(coShader);
public:
	coVulkanShader();
	virtual ~coVulkanShader();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;
	VkShaderModule shaderModule_vk;
};
