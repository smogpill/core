// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

const coChar* coGetVulkanResultString(const VkResult& _result);

#define coVULKAN_TRY(_result_vk_, _msg_) coSAFE_SCOPE(VkResult r_ = (_result_vk_); coTRY(r_ == VK_SUCCESS, _msg_ << " (Vulkan: " << coGetVulkanResultString(r_) << ").");)
