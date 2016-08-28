// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderMesh.h"
#include "render/coMesh.h"
#include "render/coRenderMaterial.h"
#include "lang/result/coResult_f.h"

coRenderMesh::coRenderMesh()
	: material(nullptr)
	, mesh(nullptr)
{

}

coRenderMesh::InitConfig::InitConfig()
	: mesh(nullptr)
	, material(nullptr)
{

}

coResult coRenderMesh::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	mesh = config.mesh;
	coTRY(mesh, nullptr);
	coTRY(mesh->IsValid(), nullptr);
	material = config.material;
	coTRY(material, nullptr);
	coTRY(material->IsValid(), nullptr);
	return true;
}

coUint coRenderMesh::GetNbIndices() const
{
	return mesh ? mesh->GetIndices().count : 0;
}

coUint coRenderMesh::GetNbVertices() const
{
	return mesh ? mesh->GetPositions().count : 0;
}