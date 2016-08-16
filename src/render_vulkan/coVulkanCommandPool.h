// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanCommandPool final : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coInt32 queueFamilyIndex;
	};

	coVulkanCommandPool();
	virtual ~coVulkanCommandPool();

	const VkCommandPool& GetVkCommandPool() const { return commandPool_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkCommandPool commandPool_vk;
};
