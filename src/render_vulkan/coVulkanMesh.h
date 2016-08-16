// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderMesh.h"

class coVulkanBuffer;

class coVulkanMesh : public coRenderMesh
{
	coDECLARE_SUPER(coRenderMesh);
public:
	coVulkanMesh();
	virtual ~coVulkanMesh();

	const coVulkanBuffer* GetVulkanBuffer() const { return vulkanBuffer; }
	coUint32 GetVertexBufferOffset() const { return 0; }
	VkDeviceSize GetIndexBufferOffset() const { return indexBufferOffset; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	const VkDevice& GetVkDevice() const;

	coVulkanBuffer* vulkanBuffer;
	VkDeviceSize indexBufferOffset;
};
