// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanRenderer.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render/coRenderEntity.h"
#include "render/coRenderWorld.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"

coVulkanRenderer::coVulkanRenderer()
	: testPipeline(nullptr)
{

}

coVulkanRenderer::~coVulkanRenderer()
{
	delete testPipeline;
}

coResult coVulkanRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coVulkanRenderer::FillCommandBuffer(const FillConfig& _config)
{
	coTRY(Super::FillCommandBuffer(_config), nullptr);

	coVulkanCommandBuffer* vulkanCommandBuffer = static_cast<coVulkanCommandBuffer*>(_config.commandBuffer);
	coTRY(vulkanCommandBuffer, nullptr);

	coVulkanFramebuffer* vulkanFramebuffer = static_cast<coVulkanFramebuffer*>(_config.framebuffer);
	coTRY(vulkanFramebuffer, nullptr);

	coVulkanPass* vulkanPass = static_cast<coVulkanPass*>(vulkanFramebuffer->GetPass());
	coTRY(vulkanPass, nullptr);

	coTRY(vulkanCommandBuffer->Start(), "Failed to start command buffer.");
	coDEFER() { vulkanCommandBuffer->Stop(); };

	vulkanCommandBuffer->PushPassBegin(*vulkanPass, *vulkanFramebuffer);
	coDEFER() { vulkanCommandBuffer->PushPassEnd(); };
	coTRY(_config.pipeline, nullptr);
	vulkanCommandBuffer->PushBindPipeline(*_config.pipeline);

	{
		coHACK("Hacked bind descriptor set.");
		coVulkanPipeline* vulkanPipeline = static_cast<coVulkanPipeline*>(_config.pipeline);
		coVulkanDescriptorSet* vulkanDescriptorSet = vulkanPipeline->GetVulkanDescriptorSet();
		const coVulkanPipelineLayout* vulkanPipelineLayout = vulkanPipeline->GetVulkanPipelineLayout();
		vulkanCommandBuffer->PushBindDescriptorSet(*vulkanDescriptorSet, *vulkanPipelineLayout, 0);
	}

	//vulkanCommandBuffer->PushDrawEmptyTriangle();

	if (_config.world)
	{
		const coArray<coRenderEntity*>& entities = _config.world->GetEntities();
		for (const coRenderEntity* entity : entities)
		{
			coASSERT(entity);
			const coRenderMesh* mesh = entity->GetRenderMesh();
			if (mesh)
			{
				vulkanCommandBuffer->PushDraw(*mesh);
			}
		}
	}

	return true;
}
