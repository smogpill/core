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
#include "math/vector/coFloatx4.h"
#include "math/matrix/coMat4_f.h"
#include "math/quaternion/coQuat_f.h"
#include "math/transform/coTransform.h"

// Hack
struct ModelConstants
{
	coMat4 modelViewProj;
};

coVulkanRenderer::coVulkanRenderer()
	: vulkanBuffer(nullptr)
	, vulkanDescriptorSet(nullptr)
{
	vulkanBuffer = new coVulkanBuffer();
	vulkanDescriptorSet = new coVulkanDescriptorSet();
}

coVulkanRenderer::~coVulkanRenderer()
{
	delete vulkanBuffer;
	delete vulkanDescriptorSet;
}

coResult coVulkanRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	{
		coHACK("Hard coded Vulkan buffer.");

		coVulkanBuffer::InitConfig c;
		c.debugName = "Hard coded Vulkan buffer";
		c.device = GetDevices()[0];
		c.size8 = sizeof(ModelConstants);
		c.type = coVulkanBuffer::dynamic;
		c.usage = coVulkanBuffer::Usage::uniform;
		coTRY(vulkanBuffer->Init(c), nullptr);

		coTRY(UpdateConstants(coVec3(0.0f)), nullptr);
	}

	{
		coHACK("Hard coded descriptor set.");
		coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(GetDevices()[0]);
		coVulkanDescriptorSet::InitConfig c;
		c.device = vulkanLogicalDevice;
		c.debugName = "Hard coded descriptor set";
		c.vulkanBuffer = vulkanBuffer;
		c.vulkanDescriptorPool = vulkanLogicalDevice->GetVulkanDescriptorPool();
		coTRY(vulkanDescriptorSet->Init(c), nullptr);
	}

	return true;
}

coResult coVulkanRenderer::FillCommandBuffer(const FillConfig& _config)
{
	coTRY(Super::FillCommandBuffer(_config), nullptr);

	static coFloat x = 0.0f;
	x += 0.01f;
	coTRY(UpdateConstants(coVec3(x)), nullptr);

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

coResult coVulkanRenderer::UpdateConstants(const coVec3& _rotation)
{
	coMat4 model;
	{
		coTransform t;
		t.rotation = coRotation(_rotation);
		coSetWithoutScale(model, t);
	}

	coMat4 view;
	{
		coTransform t;
		t.translation = coVec3(0, 0, 5);
		coSetWithoutScale(view, t);
	}

	coMat4 proj;
	{
		coSetPerspective(proj, coConvertToRad(90.0f), 16.f / 9.f, 0.05f, 1000.0f);
	}

	ModelConstants constants;
	constants.modelViewProj = proj * view * model;

	void* data = nullptr;
	coTRY(vulkanBuffer->Map(data), nullptr);
	*reinterpret_cast<ModelConstants*>(data) = constants;
	vulkanBuffer->Unmap();
	return true;
}
