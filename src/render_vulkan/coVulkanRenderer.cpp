// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanRenderer.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render_vulkan/coVulkanDescriptorSet.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render/coRenderEntity.h"
#include "render/coRenderWorld.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "math/matrix/coMat4_f.h"
#include "math/transform/coTransform.h"
#include "gui/immediate/coImGui.h"

// Hack
struct ModelConstants
{
	coMat4 modelViewProj;
};

coVulkanRenderer::coVulkanRenderer()
	: modelConstantsBuffer(nullptr)
	, vulkanDescriptorSet(nullptr)
{
	modelConstantsBuffer = new coVulkanBuffer();
	vulkanDescriptorSet = new coVulkanDescriptorSet();
}

coVulkanRenderer::~coVulkanRenderer()
{
	delete modelConstantsBuffer;
	delete vulkanDescriptorSet;
}

coResult coVulkanRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	// Init model constants buffer
	{
		coVulkanBuffer::InitConfig c;
		c.debugName = "ModelConstantsBuffer";
		c.device = GetDevices()[0];
		c.size8 = sizeof(ModelConstants);
		c.type = coVulkanBuffer::dynamic;
		c.usage = coVulkanBuffer::Usage::uniform;
		coTRY(modelConstantsBuffer->Init(c), nullptr);

		coTRY(UpdateConstants(), nullptr);
	}

	{
		coHACK("Hard coded descriptor set.");
		coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(GetDevices()[0]);
		coVulkanDescriptorSet::InitConfig c;
		c.device = vulkanLogicalDevice;
		c.debugName = "Hard coded descriptor set";
		c.vulkanBuffer = modelConstantsBuffer;
		c.vulkanDescriptorPool = vulkanLogicalDevice->GetVulkanDescriptorPool();
		coTRY(vulkanDescriptorSet->Init(c), nullptr);
	}

	return true;
}

coResult coVulkanRenderer::FillCommandBuffer(const FillConfig& _config)
{
	coTRY(Super::FillCommandBuffer(_config), nullptr);

	coTRY(UpdateConstants(), nullptr);
	coMat4 viewMatrix;
	{
		coTransform t;
		t.translation = coVec3(0, 0, 2);
		coSetWithoutScale(viewMatrix, t);
	}

	coMat4 projMatrix;
	{
		coSetPerspective(projMatrix, coConvertToRad(90.0f), 16.f / 9.f, 0.05f, 1000.0f);
	}

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

	coVulkanPipeline* vulkanPipeline = static_cast<coVulkanPipeline*>(_config.pipeline);
	const coVulkanPipelineLayout* vulkanPipelineLayout = vulkanPipeline->GetVulkanPipelineLayout();

	{
		coHACK("Hacked bind descriptor set.");
		
		vulkanCommandBuffer->PushBindDescriptorSet(*vulkanDescriptorSet, *vulkanPipelineLayout, 0);
	}

	if (_config.world)
	{
		const coArray<coRenderEntity*>& entities = _config.world->GetEntities();
		for (const coRenderEntity* entity : entities)
		{
			coASSERT(entity);
			const coMat4& modelMatrix = entity->GetWorldMatrix();
			const coMat4 modelViewProj = projMatrix * viewMatrix * modelMatrix;
			coASSERT(vulkanPipelineLayout);
			vulkanCommandBuffer->PushConstants(*vulkanPipelineLayout, 0, sizeof(modelViewProj), &modelViewProj);

			const coRenderMesh* mesh = entity->GetRenderMesh();
			if (mesh)
			{
				vulkanCommandBuffer->PushBindAndDraw(*mesh);
			}
		}
	}

	if (_config.imGui)
	{
		coTRY(_config.imGuiPipeline, nullptr);
		vulkanCommandBuffer->PushBindPipeline(*_config.imGuiPipeline);
		coImGui* imGui = _config.imGui;
		coCHECK(imGui->Render(*vulkanCommandBuffer), nullptr);
	}

	return true;
}

coResult coVulkanRenderer::UpdateConstants()
{
	ModelConstants constants;

	void* data = nullptr;
	coTRY(modelConstantsBuffer->Map(data), nullptr);
	*reinterpret_cast<ModelConstants*>(data) = constants;
	modelConstantsBuffer->Unmap();
	return true;
}
