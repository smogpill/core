// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderBuffer.h"

class coVulkanBuffer : public coRenderBuffer
{
	coDECLARE_SUPER(coRenderBuffer);
public:
	coVulkanBuffer();
	virtual ~coVulkanBuffer();

	const VkBuffer& GetVkBuffer() const { return buffer_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkBuffer buffer_vk;
};
