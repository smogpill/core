// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDescriptorSet.h"
#include "render_vulkan/coVulkanDescriptorPool.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanDescriptorSetLayout.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanDescriptorSet::coVulkanDescriptorSet()
	: descriptorSet_vk(VK_NULL_HANDLE)
	, vulkanDescriptorPool(nullptr)
	, vulkanDescriptorSetLayout(nullptr)
{
	vulkanDescriptorSetLayout = new coVulkanDescriptorSetLayout();
}

coVulkanDescriptorSet::~coVulkanDescriptorSet()
{
	if (descriptorSet_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		const VkDescriptorPool& descriptorPool_vk = GetVkDescriptorPool();
		coASSERT(descriptorPool_vk != VK_NULL_HANDLE);
		vkFreeDescriptorSets(device_vk, descriptorPool_vk, 1, &descriptorSet_vk);
	}

	delete vulkanDescriptorSetLayout;
}

coVulkanDescriptorSet::InitConfig::InitConfig()
	: vulkanBuffer(nullptr)
	, vulkanDescriptorPool(nullptr)
{

}

coResult coVulkanDescriptorSet::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coASSERT(descriptorSet_vk == VK_NULL_HANDLE);

	vulkanDescriptorPool = config.vulkanDescriptorPool;
	coTRY(vulkanDescriptorPool, nullptr);

	const VkDescriptorPool& descriptorPool_vk = GetVkDescriptorPool();
	coTRY(descriptorPool_vk != VK_NULL_HANDLE, nullptr);

	{
		coHACK("Hard coded descriptor set layout.");
		coVulkanDescriptorSetLayout::InitConfig c;
		c.debugName = "Hard coded coVulkanDescriptorSetLayout";
		c.device = config.device;
		coTRY(vulkanDescriptorSetLayout->Init(c), nullptr);
	}

	const VkDescriptorSetLayout& descriptorSetLayout_vk = vulkanDescriptorSetLayout->GetVkDescriptorSetLayout();

	VkDescriptorSetLayout layouts_vk[]{ descriptorSetLayout_vk };

	VkDescriptorSetAllocateInfo allocInfo_vk{};
	allocInfo_vk.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo_vk.descriptorPool = descriptorPool_vk;
	allocInfo_vk.descriptorSetCount = 1;
	allocInfo_vk.pSetLayouts = layouts_vk;

	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);
	coVULKAN_TRY(vkAllocateDescriptorSets(device_vk, &allocInfo_vk, &descriptorSet_vk), "Failed to allocate descriptor set: "<<*this);

	coTRY(config.vulkanBuffer, nullptr);
	const VkBuffer& buffer_vk = config.vulkanBuffer->GetVkBuffer();

	VkDescriptorBufferInfo bufferInfo_vk{};
	bufferInfo_vk.buffer = buffer_vk;
	bufferInfo_vk.offset = 0;
	bufferInfo_vk.range = config.vulkanBuffer->GetSize8();

	VkWriteDescriptorSet descriptorWrite_vk{};
	descriptorWrite_vk.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite_vk.dstSet = descriptorSet_vk;
	descriptorWrite_vk.dstBinding = 0;
	descriptorWrite_vk.dstArrayElement = 0;
	descriptorWrite_vk.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite_vk.descriptorCount = 1;
	descriptorWrite_vk.pBufferInfo = &bufferInfo_vk;
	descriptorWrite_vk.pImageInfo = nullptr; // Optional
	descriptorWrite_vk.pTexelBufferView = nullptr; // Optional

	vkUpdateDescriptorSets(device_vk, 1, &descriptorWrite_vk, 0, nullptr);

	return true;
}

const VkDevice& coVulkanDescriptorSet::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}

const VkDescriptorPool& coVulkanDescriptorSet::GetVkDescriptorPool() const
{
	static VkDescriptorPool nullDescriptorPool_vk = VK_NULL_HANDLE;
	return vulkanDescriptorPool ? vulkanDescriptorPool->GetVkDescriptorPool() : nullDescriptorPool_vk;
}