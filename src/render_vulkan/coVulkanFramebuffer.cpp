// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanFramebuffer.h"
#include "render_vulkan/coVulkanImageView.h"
#include "render_vulkan/coVulkanPass.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanFramebuffer::coVulkanFramebuffer()
	: framebuffer_vk(VK_NULL_HANDLE)
{

}

coVulkanFramebuffer::~coVulkanFramebuffer()
{
	if (framebuffer_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk);
		vkDestroyFramebuffer(device_vk, framebuffer_vk, nullptr);
	}
}

coResult coVulkanFramebuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	coVulkanPass* vulkanPass = static_cast<coVulkanPass*>(config.pass);
	coTRY(vulkanPass, nullptr);
	const VkRenderPass& renderPass_vk = vulkanPass->GetVkRenderPass();
	coTRY(renderPass_vk != VK_NULL_HANDLE, nullptr);
	coTRY(config.size.x > 0, nullptr);
	coTRY(config.size.y > 0, nullptr);

	coDynamicArray<VkImageView> imageViews_vk;
	coReserve(imageViews_vk, config.imageViews.count);
	for (coRenderImageView* view : config.imageViews)
	{
		coVulkanImageView* vulkanView = static_cast<coVulkanImageView*>(view);
		coTRY(vulkanView, nullptr);
		const VkImageView& imageView_vk = vulkanView->GetVkImageView();
		coTRY(imageView_vk != VK_NULL_HANDLE, nullptr);
		coPushBack(imageViews_vk, imageView_vk);
	}

	VkFramebufferCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.renderPass = renderPass_vk;
	createInfo.attachmentCount = imageViews_vk.count;
	createInfo.pAttachments = imageViews_vk.data;
	createInfo.width = config.size.x;
	createInfo.height = config.size.y;
	createInfo.layers = 1;

	coASSERT(framebuffer_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateFramebuffer(device_vk, &createInfo, nullptr, &framebuffer_vk), "Failed to create framebuffer.");
	return true;
}