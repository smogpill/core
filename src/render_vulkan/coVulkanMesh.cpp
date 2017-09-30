// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render_vulkan/pch.h"
#include "render_vulkan/coVulkanMesh.h"
#include "render_vulkan/coVulkanBuffer.h"
#include "render/coMesh.h"
#include "render/coRenderVertexChannels.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"

coVulkanMesh::coVulkanMesh()
	: vulkanBuffer(nullptr)
	, indexBufferOffset(0)
{

}

coVulkanMesh::~coVulkanMesh()
{
	delete vulkanBuffer;
}

coResult coVulkanMesh::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	coASSERT(mesh);
	const auto& positions = mesh->GetPositions();
	const auto& normals = mesh->GetNormals();
	const auto& tangents = mesh->GetTangents();
	const auto& uvs = mesh->GetUvs();
	const auto& indexBuffer = mesh->GetIndexBuffer();
	const coUint32 vertexBufferSize = positions.count * sizeof(coRenderVertex_PosNormalTangentUv);
	const coUint32 indexBufferSize = indexBuffer.count * sizeof(indexBuffer[0]);

	coTRY(mesh->indexSize8 == 2 || mesh->indexSize8 == 4, "Only 16 or 32 bits indices supported.");

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
		coRenderVertex_PosNormalTangentUv* posData = static_cast<coRenderVertex_PosNormalTangentUv*>(data);
		const coUint32 nbVertices = positions.count;
		for (coUint i = 0; i < nbVertices; ++i)
		{
			posData->pos = positions[i];
			posData->normal = normals[i];
			posData->tangent = tangents[i];
			posData->uv = uvs[i];
			++posData;
		}
		coByte* index = static_cast<coByte*>(data) + indexBufferOffset;
		coMemCopy(index, indexBuffer.data, indexBufferSize);
	}

	coSwap(vulkanBuffer, b);
	return true;
}

VkIndexType coVulkanMesh::GetVkIndexType() const
{
	switch (mesh->indexSize8)
	{
	case 2: return VK_INDEX_TYPE_UINT16;
	case 4: return VK_INDEX_TYPE_UINT32;
	default:
	{
		coERROR("Unsupported index type: "<<mesh->indexSize8<<".");
		return VK_INDEX_TYPE_MAX_ENUM;
	}
	}
}
