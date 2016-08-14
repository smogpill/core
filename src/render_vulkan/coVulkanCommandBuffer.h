// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanCommandBuffer final : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	coVulkanCommandBuffer();
	virtual ~coVulkanCommandBuffer();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	VkCommandBuffer commandBuffer_vk;
};
