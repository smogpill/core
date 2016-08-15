// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coRenderPass;
class coRenderFramebuffer;
class coRenderPipeline;

class coVulkanCommandBuffer final : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	coVulkanCommandBuffer();
	virtual ~coVulkanCommandBuffer();

	void PushPassBegin(const coRenderPass& _pass, const coRenderFramebuffer& _frameBuffer);
	void PushPassEnd();
	void PushBindPipeline(const coRenderPipeline& _pipeline);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop();

private:
	VkCommandBuffer commandBuffer_vk;
	coBool passStarted;
};
