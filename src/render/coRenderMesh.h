// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coFloatx2.h"
#include "math/vector/coFloatx3.h"

class coRenderMaterial;
class coMesh;

class coRenderMesh : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coMesh* mesh;
		coRenderMaterial* material;
	};

	coMesh* GetMesh() const { return mesh; }
	coRenderMaterial* GetMaterial() const { return material; }
	coUint GetNbIndices() const;
	coUint GetNbVertices() const;

protected:
	coRenderMesh();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coMesh* mesh;
	coRenderMaterial* material;
};
