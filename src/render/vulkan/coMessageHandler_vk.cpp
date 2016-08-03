// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/vulkan/coMessageHandler_vk.h"
#include "render/vulkan/coResult_f_vk.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "memory/allocator/coLocalAllocator.h"

static VkBool32 coDebugCallback_vk(VkDebugReportFlagsEXT _flags, VkDebugReportObjectTypeEXT /*_objType*/, uint64_t /*_obj*/,
	size_t /*_location*/, int32_t /*_code*/, const char* _layerPrefix, const char* _msg, void* /*_userData*/)
{
	coLocalAllocator localAllocator(2048);
	coDynamicString prefix(localAllocator);
	prefix << "Vulkan";
	if (_layerPrefix)
	{
		prefix << " (layer: " << _layerPrefix << ")";
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

coMessageHandler_vk::InitConfig::InitConfig()
	: instance_vk(VK_NULL_HANDLE)
{

}

coMessageHandler_vk::coMessageHandler_vk()
	: instance_vk(VK_NULL_HANDLE)
	, callback_vk(VK_NULL_HANDLE)
{

}

coMessageHandler_vk::~coMessageHandler_vk()
{
	if (callback_vk != VK_NULL_HANDLE)
	{
		coASSERT(instance_vk != VK_NULL_HANDLE);
		auto vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(*instance_vk, "vkDestroyDebugReportCallbackEXT"));
		vkDestroyDebugReportCallbackEXT(*instance_vk, callback_vk, nullptr);
	}
}

coResult coMessageHandler_vk::OnInit(const coObject::InitConfig& _config)
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
	createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)coDebugCallback_vk;
	coTRY_vk(createFunc(*instance_vk, &createInfo, nullptr, &callback_vk), "Failed to create the message handler callback");
	return true;
}
