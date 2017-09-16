// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanDescriptorSetLayout : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
	};
	coVulkanDescriptorSetLayout();
	virtual ~coVulkanDescriptorSetLayout();

	const VkDescriptorSetLayout& GetVkDescriptorSetLayout() const { return descriptorSetLayout_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkDescriptorSetLayout descriptorSetLayout_vk;
};
