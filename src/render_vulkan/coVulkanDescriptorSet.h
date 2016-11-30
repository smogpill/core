// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanDescriptorPool;
class coVulkanBuffer;

class coVulkanDescriptorSet : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coVulkanBuffer* vulkanBuffer;
	};
	coVulkanDescriptorSet();
	virtual ~coVulkanDescriptorSet();

	const VkDescriptorSet& GetVkDescriptorSet() const { return descriptorSet_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;
	const VkDescriptorPool& GetVkDescriptorPool() const;

	coVulkanDescriptorPool* vulkanDescriptorPool;
	VkDescriptorSet descriptorSet_vk;
};
