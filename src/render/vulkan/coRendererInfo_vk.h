// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coMessageHandler_vk;

class coRendererInfo_vk
{
public:
	coRendererInfo_vk();
	VkInstance instance_vk;
	VkAllocationCallbacks allocator_vk;
	coBool enableDebug;
	coMessageHandler_vk* messageHandler_vk;
};
