// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanCommandPool.h"
#include "render_vulkan/coVulkanPipeline.h"
#include "render_vulkan/coVulkanMesh.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coVulkanCommandBuffer::coVulkanCommandBuffer()
	: commandBuffer_vk(VK_NULL_HANDLE)
	, passStarted(false)
	, vulkanCommandPool(nullptr)
{

}

coVulkanCommandBuffer::~coVulkanCommandBuffer()
{
	if (commandBuffer_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		const VkCommandPool& commandPool_vk = GetVkCommandPool();
		vkFreeCommandBuffers(device_vk, commandPool_vk, 1, &commandBuffer_vk);
	}
}

coResult coVulkanCommandBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config); 
	coTRY(device, nullptr);
	coVulkanLogicalDevice* vulkanDevice = static_cast<coVulkanLogicalDevice*>(device);
	const VkDevice& device_vk = vulkanDevice->GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	vulkanCommandPool = vulkanDevice->GetVulkanCommandPool(coVulkanLogicalDevice::graphics);
	coTRY(vulkanCommandPool, nullptr);
	const VkCommandPool& commandPool_vk = vulkanCommandPool->GetVkCommandPool();
	coTRY(commandPool_vk != VK_NULL_HANDLE, nullptr);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool_vk;
	allocInfo.level = config.primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocInfo.commandBufferCount = 1;

	coASSERT(commandBuffer_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkAllocateCommandBuffers(device_vk, &allocInfo, &commandBuffer_vk), "Failed to allocate command buffer.");

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

	VkClearValue clearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
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

void coVulkanCommandBuffer::PushExecuteCommands(const coArray<const coRenderCommandBuffer*>& _commandBuffers)
{
	coASSERT(IsStarted());
	coASSERT(passStarted);
	coASSERT(commandBuffer_vk != VK_NULL_HANDLE);

	coDynamicArray<VkCommandBuffer> commandBuffers_vk;
	coReserve(commandBuffers_vk, _commandBuffers.count);
	for (const coRenderCommandBuffer* commandBuffer : _commandBuffers)
	{
		const coVulkanCommandBuffer* vulkanCommandBuffer = static_cast<const coVulkanCommandBuffer*>(commandBuffer);
		coASSERT(vulkanCommandBuffer);
		const VkCommandBuffer& cb_vk = vulkanCommandBuffer->GetVkCommandBuffer();
		coASSERT(cb_vk != VK_NULL_HANDLE);
		coPushBack(commandBuffers_vk, cb_vk);
	}
	vkCmdExecuteCommands(commandBuffer_vk, commandBuffers_vk.count, commandBuffers_vk.data);
}

void coVulkanCommandBuffer::PushDraw(const coRenderMesh& _mesh)
{
	coASSERT(IsStarted());
	coASSERT(passStarted);
	coASSERT(commandBuffer_vk != VK_NULL_HANDLE);

	const coVulkanMesh& vulkanMesh = static_cast<const coVulkanMesh&>(_mesh);
	const coVulkanBuffer* vulkanBuffer = vulkanMesh.GetVulkanBuffer();
	coASSERT(vulkanBuffer);
	const VkBuffer& buffer_vk = vulkanBuffer->GetVkBuffer();
	coASSERT(buffer_vk != VK_NULL_HANDLE);
	VkDeviceSize offset_vk = 0;
	vkCmdBindVertexBuffers(commandBuffer_vk, 0, 1, &buffer_vk, &offset_vk);
	if (_mesh.indices.count)
	{
		vkCmdBindIndexBuffer(commandBuffer_vk, buffer_vk, vulkanMesh.GetIndexBufferOffset(), VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(commandBuffer_vk, _mesh.indices.count, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer_vk, _mesh.positions.count, 1, 0, 0);
	}
}

const VkCommandPool& coVulkanCommandBuffer::GetVkCommandPool() const
{
	static VkCommandPool nullCommandPool_vk = VK_NULL_HANDLE;
	return vulkanCommandPool ? vulkanCommandPool->GetVkCommandPool() : nullCommandPool_vk;
}

const VkDevice& coVulkanCommandBuffer::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

coBool coVulkanCommandBuffer::IsValid() const
{
	if (!Super::IsValid())
		return false;

	return commandBuffer_vk != VK_NULL_HANDLE && !passStarted;
}
