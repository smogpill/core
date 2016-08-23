// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanShader.h"
#include "render_vulkan/coVulkanLogicalDevice.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "lang/result/coResult_f.h"

coVulkanShader::coVulkanShader()
	: shaderModule_vk(VK_NULL_HANDLE)
{

}

coVulkanShader::~coVulkanShader()
{
	if (shaderModule_vk != VK_NULL_HANDLE)
	{
		const VkDevice& device_vk = GetVkDevice();
		coASSERT(device_vk != VK_NULL_HANDLE);
		vkDestroyShaderModule(device_vk, shaderModule_vk, nullptr);
	}
}

const VkDevice& coVulkanShader::GetVkDevice() const
{
	coVulkanLogicalDevice* vulkanLogicalDevice = static_cast<coVulkanLogicalDevice*>(device);
	static VkDevice nullDevice = VK_NULL_HANDLE;
	return vulkanLogicalDevice ? vulkanLogicalDevice->GetVkDevice() : nullDevice;
}

coResult coVulkanShader::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = config.codeSize8;
	createInfo.pCode = static_cast<const uint32_t*>(config.code);

	const VkDevice& device_vk = GetVkDevice();
	coTRY(device_vk != VK_NULL_HANDLE, nullptr);

	coASSERT(shaderModule_vk == VK_NULL_HANDLE);
	coVULKAN_TRY(vkCreateShaderModule(device_vk, &createInfo, nullptr, &shaderModule_vk), "Failed to create Vulkan shader module.");
	return true;
}

VkShaderStageFlagBits coVulkanShader::GetVkShaderStageFlagBits() const
{
	VkShaderStageFlagBits stages_vk;
	switch (stage)
	{
	case Stage::vertex: stages_vk = VK_SHADER_STAGE_VERTEX_BIT; break;
	case Stage::tesselationControl: stages_vk = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
	case Stage::tesselationEvaluation: stages_vk = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
	case Stage::geometry: stages_vk = VK_SHADER_STAGE_GEOMETRY_BIT; break;
	case Stage::fragment: stages_vk = VK_SHADER_STAGE_FRAGMENT_BIT; break;
	case Stage::compute: stages_vk = VK_SHADER_STAGE_COMPUTE_BIT; break;
	default: stages_vk = static_cast<VkShaderStageFlagBits>(0); break;
	}

	return stages_vk;
}
