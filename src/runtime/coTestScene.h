// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderWorld;
class coRenderEntity;
class coRenderMesh;
class coRenderDevice;
class coMesh;
class coRenderer;
class coShader;
class coRenderMaterial;

class coTestScene : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coRenderDevice* renderDevice;
	};
	coTestScene();
	virtual ~coTestScene();

	const coArray<coRenderEntity*>& GetEntities() const { return entities; }
	coResult Update();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitMesh();
	coResult InitRenderMesh();
	coResult InitRenderEntities();
	coResult InitShaders();
	coResult InitMaterial();

	coRenderDevice* renderDevice;
	coMesh* mesh;
	coRenderMesh* renderMesh;
	coShader* vertexShader;
	coShader* fragmentShader;
	coRenderMaterial* material;
	coDynamicArray<coRenderEntity*> entities;
};
