// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderCommandBuffer.h"

class coRenderPass;
class coRenderFramebuffer;
class coRenderPipeline;
class coShader;
class coRenderMesh;
class coVulkanCommandPool;

class coVulkanCommandBuffer final : public coRenderCommandBuffer
{
	coDECLARE_SUPER(coRenderCommandBuffer);
public:
	coVulkanCommandBuffer();
	virtual ~coVulkanCommandBuffer();

	void PushPassBegin(const coRenderPass& _pass, const coRenderFramebuffer& _frameBuffer);
	void PushPassEnd();
	void PushBindPipeline(const coRenderPipeline& _pipeline);
	void PushExecuteCommands(const coArray<const coRenderCommandBuffer*>& _commandBuffers);
	void PushDraw(const coRenderMesh& _mesh);
	void PushDrawEmptyTriangle();
	const VkCommandBuffer& GetVkCommandBuffer() const { return commandBuffer_vk; }
	virtual coBool IsValid() const override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop();

private:
	const VkCommandPool& GetVkCommandPool() const;
	const VkDevice& GetVkDevice() const;

	VkCommandBuffer commandBuffer_vk;
	coVulkanCommandPool* vulkanCommandPool;
	coBool passStarted;
};
