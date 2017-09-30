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
class coVulkanDescriptorSet;
class coVulkanPipeline;
class coVulkanPipelineLayout;
class coInt32x4;

class coVulkanCommandBuffer final : public coRenderCommandBuffer
{
	coDECLARE_SUPER(coRenderCommandBuffer);
public:
	coVulkanCommandBuffer();
	virtual ~coVulkanCommandBuffer();

	void PushPassBegin(const coRenderPass& _pass, const coRenderFramebuffer& _frameBuffer);
	void PushPassEnd();
	void PushBindPipeline(const coRenderPipeline& _pipeline);
	virtual void PushExecuteCommands(const coArray<const coRenderCommandBuffer*>& _commandBuffers) override;
	void PushBindDescriptorSet(const coVulkanDescriptorSet& _vulkanDescriptorSet, const coVulkanPipelineLayout& _vulkanPipelineLayout, coUint _index);
	void PushConstants(const coVulkanPipelineLayout& _vulkanPipelineLayout, coUint _offset, coUint _size, const void* _data);
	virtual void PushDraw(const coRenderMesh& _mesh) override;
	void PushDrawEmptyTriangle();
	virtual void PushSetScissor(coUint32 _x, coUint32 _y, coUint32 _width, coUint32 _height) override;
	const VkCommandBuffer& GetVkCommandBuffer() const { return commandBuffer_vk; }
	virtual coBool IsValid() const override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;
	virtual void OnStop();

private:
	const VkCommandPool& GetVkCommandPool() const;
	const VkDevice& GetVkDevice() const;
	coBool CheckReadyForPassCommands() const;

	VkCommandBuffer commandBuffer_vk;
	coVulkanCommandPool* vulkanCommandPool;
	coBool passStarted;
};
