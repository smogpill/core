// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coVulkanDeviceMemoryChunk;
class coVulkanDeviceAllocation;

class coVulkanDeviceAllocator : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	enum Type
	{
		deviceLocal,
		uploadToDevice,
		readBackFromDevice,
		end
	};
	virtual ~coVulkanDeviceAllocator();

	coResult Allocate(coVulkanDeviceAllocation*& _alloc, const VkMemoryPropertyFlags& _flags_vk, const VkDeviceSize& _size_vk, const VkDeviceSize& _alignment);
	void Free(coVulkanDeviceAllocation& _alloc);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;
	coResult AllocateChunk(coVulkanDeviceMemoryChunk*& _chunk, const VkMemoryPropertyFlags& _requiredFlags_vk, const VkDeviceSize& _size_vk);

	coDynamicArray<coVulkanDeviceMemoryChunk*> chunks;
};
