// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanMesh.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render/coMesh.h"
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

	coASSERT(mesh);
	const auto& positions = mesh->GetPositions();
	const auto& indices = mesh->GetIndices();
	const coUint32 vertexBufferSize = positions.count * sizeof(_coVertex_PosNormalUv);
	const coUint32 indexBufferSize = indices.count * sizeof(indices[0]);

	coVulkanBuffer* b = new coVulkanBuffer();
	coDEFER() { delete b; };
	coVulkanBuffer::InitConfig c;
	c.device = device;
	c.debugName = "MeshVulkanBuffer";
	c.usage = coVulkanBuffer::index;
	c.type = coVulkanBuffer::dynamic; // temp
	c.size8 = vertexBufferSize + indexBufferSize;
	coTRY(b->Init(c), "Failed to init the vulkan buffer.");
	indexBufferOffset = vertexBufferSize;

	// Copy data
	void* data = nullptr;
	coTRY(b->Map(data), "Failed to map the vulkan buffer data.");
	{
		coDEFER() { b->Unmap(); };
		_coVertex_PosNormalUv* posData = static_cast<_coVertex_PosNormalUv*>(data);
		for (const auto& p : positions)
		{
			posData->pos = p;
			posData->normal = coFloatx3(0);
			posData->uv = coFloatx2(0);
			++posData;
		}
		coByte* index = static_cast<coByte*>(data) + indexBufferOffset;
		coMemCopy(index, indices.data, indexBufferSize);
	}

	coSwap(vulkanBuffer, b);
	return true;
}
