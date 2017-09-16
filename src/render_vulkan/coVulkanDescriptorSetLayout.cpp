// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanDescriptorSetLayout.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanDescriptorSetLayout::coVulkanDescriptorSetLayout()
	: descriptorSetLayout_vk(VK_NULL_HANDLE)
{

}

coVulkanDescriptorSetLayout::~coVulkanDescriptorSetLayout()
{
	if (descriptorSetLayout_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		vkDestroyDescriptorSetLayout(device_vk, descriptorSetLayout_vk, nullptr);
	}
}

coVulkanDescriptorSetLayout::InitConfig::InitConfig()
{

}

coResult coVulkanDescriptorSetLayout::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);

	coHACK("hard coded VkDescriptorSetLayoutBindings.");

	VkDescriptorSetLayoutBinding binding_vk{};
	binding_vk.binding = 0;
	binding_vk.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	binding_vk.descriptorCount = 1;
	binding_vk.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	binding_vk.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo_vk{};
	layoutInfo_vk.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo_vk.bindingCount = 1;
	layoutInfo_vk.pBindings = &binding_vk;

	coVULKAN_TRY(vkCreateDescriptorSetLayout(device_vk, &layoutInfo_vk, nullptr, &descriptorSetLayout_vk), "Failed to create descriptor set layout: " << *this);

	return true;
}

const VkDevice& coVulkanDescriptorSetLayout::GetVkDevice() const
{
	const coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<const coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice_vk = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice_vk;
}
