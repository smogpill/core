// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanMessageHandler.h"
#include "render_vulkan/coVulkanResult_f.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "memory/allocator/coLocalAllocator.h"

static const coConstString _coGetDebugReportObjectTypeString(const VkDebugReportObjectTypeEXT& _type)
{
	switch (_type)
	{
	case VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT: return "VkInstance";
	case VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT: return "VkPhysicalDevice";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT: return "VkDevice";
	case VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT: return "VkQueue";
	case VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT: return "VkSemaphore";
	case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT: return "VkCommandBuffer";
	case VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT: return "VkFence";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT: return "VkDeviceMemory";
	case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT: return "VkBuffer";
	case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT: return "VkImage";
	case VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT: return "VkEvent";
	case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT: return "VkQueryPool";
	case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT: return "VkBufferView";
	case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT: return "VkImageView";
	case VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT: return "VkShaderModule";
	case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT: return "VkPipelineCache";
	case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT: return "VkPipelineLayout";
	case VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT: return "VkRenderPass";
	case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT: return "VkPipeline";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT: return "VkDescriptorSetLayout";
	case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT: return "VkSampler";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT: return "VkDescriptorPool";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT: return "VkDescriptorSet";
	case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT: return "VkFramebuffer";
	case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT: return "VkCommandPool";
	case VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT: return "VkSurfaceKHR";
	case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT: return "VkSwapchainKHR";
	case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT: return "VkDebugReport";
	default: return "<unknown>";
	}
}

static VkBool32 _coDebugCallback_vk(VkDebugReportFlagsEXT _flags, VkDebugReportObjectTypeEXT _objType, uint64_t /*_obj*/,
	size_t /*_location*/, int32_t /*_code*/, const char* _layerPrefix, const char* _msg, void* /*_userData*/)
{
	coLocalAllocator localAllocator(2048);
	coDynamicString prefix(localAllocator);
	prefix << "Vulkan";
	if (_layerPrefix)
	{
		prefix << " (objectType: " << _coGetDebugReportObjectTypeString(_objType) << ", layer: " << _layerPrefix << ") ";
	}

	if (_flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		coERROR(prefix << _msg);
	}
	else if (_flags & VK_DEBUG_REPORT_WARNING_BIT_EXT || _flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		coWARN(prefix << _msg);
	}
	else
	{
		coINFO(prefix << _msg);
	}

	return VK_FALSE;
}

coVulkanMessageHandler::InitConfig::InitConfig()
	: instance_vk(VK_NULL_HANDLE)
{

}

coVulkanMessageHandler::coVulkanMessageHandler()
	: instance_vk(VK_NULL_HANDLE)
	, callback_vk(VK_NULL_HANDLE)
{

}

coVulkanMessageHandler::~coVulkanMessageHandler()
{
	if (callback_vk != VK_NULL_HANDLE)
	{
		coASSERT(instance_vk != VK_NULL_HANDLE);
		auto vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(*instance_vk, "vkDestroyDebugReportCallbackEXT"));
		vkDestroyDebugReportCallbackEXT(*instance_vk, callback_vk, nullptr);
	}
}

coResult coVulkanMessageHandler::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coTRY(config.instance_vk, nullptr);
	auto createFunc = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(*config.instance_vk, "vkCreateDebugReportCallbackEXT"));
	coTRY(createFunc, "Failed to retrieve PFN_vkCreateDebugReportCallbackEXT");
	instance_vk = config.instance_vk;
	coASSERT(callback_vk == VK_NULL_HANDLE);

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)_coDebugCallback_vk;
	coVULKAN_TRY(createFunc(*instance_vk, &createInfo, nullptr, &callback_vk), "Failed to create the message handler callback");
	return true;
}
