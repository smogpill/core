// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coTestScene.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "render/coRenderWorld.h"
#include "render/coRenderEntity.h"
#include "render/coRenderMesh.h"
#include "render/coRenderFactory.h"
#include "render/coShader.h"
#include "render/coRenderMaterial.h"
#include "render/coMesh.h"
#include "io/file/coFileAccess.h"
#include "io/file/coFile_f.h"
#include "container/array/coDynamicArray_f.h"

coTestScene::coTestScene()
	: mesh(nullptr)
	, renderMesh(nullptr)
	, vertexShader(nullptr)
	, fragmentShader(nullptr)
	, material(nullptr)
	, renderDevice(nullptr)
{

}

coTestScene::~coTestScene()
{
	for (coRenderEntity* p : entities)
		delete p;
	delete renderMesh;
	delete mesh;
	delete material;
	delete fragmentShader;
	delete vertexShader;
}

coTestScene::InitConfig::InitConfig()
	: renderDevice(nullptr)
{

}

coResult coTestScene::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	renderDevice = config.renderDevice;
	coTRY(renderDevice, nullptr);
	coTRY(InitShaders(), "Failed to init the shaders.");
	coTRY(InitMaterial(), "Failed to init the material.");
	coTRY(InitMesh(), "Failed to init the mesh.");
	coTRY(InitRenderMesh(), "Failed to init the render mesh.");
	coTRY(InitRenderEntities(), "Failed to init the render entities.");
	return true;
}

coResult coTestScene::InitRenderEntities()
{
	coRenderEntity* p = new coRenderEntity();
	coDEFER() { delete p; };
	coRenderEntity::InitConfig c;
	c.debugName = "TestRenderEntity";
	coTRY(p->Init(c), "Failed to init the render entity.");
	coTRY(p->SetRenderMesh(renderMesh), "Failed to set the render mesh to the render entity.");
	coPushBack(entities, p);
	p = nullptr;
	return true;
}

coResult coTestScene::InitMesh()
{
	coMesh* p = new coMesh();
	coDEFER() { delete p; };

	const coFloat halfSize = 0.5f;
	const coFloatx3 positions[] = 
	{
		coFloatx3(-halfSize, -halfSize, -halfSize),
		coFloatx3(halfSize, -halfSize, -halfSize),
		coFloatx3(-halfSize, halfSize, -halfSize),
		coFloatx3(halfSize, halfSize, -halfSize),
		coFloatx3(-halfSize, -halfSize, halfSize),
		coFloatx3(halfSize, -halfSize, halfSize),
		coFloatx3(-halfSize, halfSize, halfSize),
		coFloatx3(halfSize, halfSize, halfSize),
	};
	coPushBackArray(p->positions, coArray<coFloatx3>(positions));
	coSwap(mesh, p);
	return true;
}

coResult coTestScene::InitRenderMesh()
{
	coRenderMesh* p = coCreateRenderMesh();
	coDEFER() { delete p; };
	coRenderMesh::InitConfig c;
	c.debugName = "RenderMesh";
	c.device = renderDevice;
	c.material = material;
	c.mesh = mesh;
	coTRY(p->Init(c), "Failed to init the render mesh.");
	coSwap(renderMesh, p);
	return true;
}

coResult coTestScene::InitShaders()
{
	coTRY(renderDevice, nullptr);
	coDynamicArray<coUint8> code;
	{
		coTRY(coReadFullFile(code, "shaders/test.vert.spv"), nullptr);
		coShader* shader = coCreateShader();
		coDEFER() { delete shader; };
		coShader::InitConfig c;
		c.device = renderDevice;
		c.code = code;
		c.stage = coShader::vertex;
		coTRY(shader->Init(c), "Failed to init the vertex shader.");
		coSwap(vertexShader, shader);
	}

	{
		coTRY(coReadFullFile(code, "shaders/test.frag.spv"), nullptr);
		coShader* shader = coCreateShader();
		coDEFER() { delete shader; };
		coShader::InitConfig c;
		c.device = renderDevice;
		c.code = code;
		c.stage = coShader::fragment;
		coTRY(shader->Init(c), "Failed to init the fragment shader.");
		coSwap(fragmentShader, shader);
	}

	return true;
}

coResult coTestScene::InitMaterial()
{
	coRenderMaterial* m = new coRenderMaterial();
	coDEFER() { delete m; };
	coRenderMaterial::InitConfig c;
	c.vertexShader = vertexShader;
	c.fragmentShader = fragmentShader;
	coTRY(m->Init(c), "Failed to init material.");
	coSwap(m, material);
	return true;
}
