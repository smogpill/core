// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanMesh.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"

class _coVertex_PosNormalUv
{
public:
	coFloatx3 pos;
	coFloatx3 normal;
	coFloatx2 uv;
};

coVulkanMesh::coVulkanMesh()
	: vulkanBuffer(nullptr)
	, indexBufferOffset(0)
{

}

coVulkanMesh::~coVulkanMesh()
{

}

coResult coVulkanMesh::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	const coUint32 vertexBufferSize = positions.count * sizeof(_coVertex_PosNormalUv);
	const coUint32 indexBufferSize = indices.count * sizeof(coUint16);

	coVulkanBuffer* b = new coVulkanBuffer();
	coDEFER() { delete b; };
	coVulkanBuffer::InitConfig c;
	c.device = device;
	c.debugName = "Index buffer";
	c.usage = coVulkanBuffer::index;
	c.size8 = vertexBufferSize + indexBufferSize;
	coTRY(b->Init(c), "Failed to init the vulkan buffer.");
	coSwap(vulkanBuffer, b);
	indexBufferOffset = vertexBufferSize;

	return true;
}
