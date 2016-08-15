// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coVulkanCommandBuffer::coVulkanCommandBuffer()
	: commandBuffer_vk(VK_NULL_HANDLE)
	, passStarted(false)
{

}

coVulkanCommandBuffer::~coVulkanCommandBuffer()
{
	if (commandBuffer_vk != VK_NULL_HANDLE)
	{

	}
}

coResult coVulkanCommandBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);


	return true;
}

coResult coVulkanCommandBuffer::OnStart()
{
	coTRY(Super::OnStart(), nullptr);
	coTRY(commandBuffer_vk != VK_NULL_HANDLE, nullptr);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;
	coVULKAN_TRY(vkBeginCommandBuffer(commandBuffer_vk, &beginInfo), "Failed to begin the Vulkan command buffer.");

	return true;
}

void coVulkanCommandBuffer::OnStop()
{
	coVULKAN_CHECK(vkEndCommandBuffer(commandBuffer_vk), "Failed to end the Vulkan command buffer.");
	Super::OnStop();
}

void coVulkanCommandBuffer::PushPassBegin(const coRenderPass& _pass, const coRenderFramebuffer& _frameBuffer)
{
	coASSERT(IsStarted());
	coCHECK(!passStarted, nullptr);
	const coVulkanPass& vulkanPass = static_cast<const coVulkanPass&>(_pass);
	const VkRenderPass& renderPass_vk = vulkanPass.GetVkRenderPass();
	coCHECK(renderPass_vk != VK_NULL_HANDLE, nullptr);

	const coVulkanFramebuffer& vulkanFramebuffer = static_cast<const coVulkanFramebuffer&>(_frameBuffer);
	const VkFramebuffer& framebuffer_vk = vulkanFramebuffer.GetVkFramebuffer();
	coCHECK(framebuffer_vk != VK_NULL_HANDLE, nullptr);

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass_vk;
	renderPassInfo.framebuffer = framebuffer_vk;

	const coInt32x2 size = _frameBuffer.GetSize();
	coASSERT(size.x >= 0);
	coASSERT(size.y >= 0);

	renderPassInfo.renderArea.offset = { 0, 0 };
	coNumericConvert(renderPassInfo.renderArea.extent.width, size.x);
	coNumericConvert(renderPassInfo.renderArea.extent.height, size.y);

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	coCHECK(commandBuffer_vk != VK_NULL_HANDLE, nullptr);
	vkCmdBeginRenderPass(commandBuffer_vk, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	passStarted = true;
}

void coVulkanCommandBuffer::PushPassEnd()
{
	coASSERT(IsStarted());
	coCHECK(passStarted, nullptr);
	coASSERT(commandBuffer_vk != VK_NULL_HANDLE);
	passStarted = false;
	vkCmdEndRenderPass(commandBuffer_vk);
}

void coVulkanCommandBuffer::PushBindPipeline(const coRenderPipeline& _pipeline)
{
	coASSERT(IsStarted());
	coCHECK(passStarted, nullptr);
	coASSERT(commandBuffer_vk != VK_NULL_HANDLE);

	const coVulkanPipeline& vulkanPipeline = static_cast<const coVulkanPipeline&>(_pipeline);
	const VkPipeline& pipeline_vk = vulkanPipeline.GetVkPipeline();
	coASSERT(pipeline_vk != VK_NULL_HANDLE);
	vkCmdBindPipeline(commandBuffer_vk, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_vk);
}
