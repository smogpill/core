// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderSemaphore.h"

class coVulkanSemaphore : public coRenderSemaphore
{
	coDECLARE_SUPER(coRenderSemaphore);
public:
	coVulkanSemaphore();
	virtual ~coVulkanSemaphore();

	const VkSemaphore& GetVkSemaphore() const { return semaphore_vk; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	VkSemaphore semaphore_vk;
};
