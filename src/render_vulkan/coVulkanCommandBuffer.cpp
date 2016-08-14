// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanCommandBuffer.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanCommandBuffer::coVulkanCommandBuffer()
	: commandBuffer_vk(VK_NULL_HANDLE)
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