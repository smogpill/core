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
#include "math/matrix/coMat4_f.h"
#include "math/transform/coTransform.h"

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

coResult coTestScene::Update()
{
	coMat4 model;
	{
		static coFloat x = 0.0f;
		x += 0.01f;
		coTransform t;
		t.rotation = coRotation(x);
		coSetWithoutScale(model, t);
	}

	for (coRenderEntity* p : entities)
		p->SetWorldMatrix(model);

	return true;
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

	const coFloat s = 0.5f;

	const coFloatx3 v[] =
	{
		coFloatx3(-s, s, s),
		coFloatx3(s, s, s),
		coFloatx3(s, -s, s),
		coFloatx3(-s, -s, s),
		coFloatx3(-s, s, -s),
		coFloatx3(s, s, -s),
		coFloatx3(s, -s, -s),
		coFloatx3(-s, -s, -s)
	};

	const coFloatx3 n[] =
	{
		coFloatx3(0, 0, 1),
		coFloatx3(-1, 0, 0),
		coFloatx3(0, 1, 0),
		coFloatx3(1, 0, 0),
		coFloatx3(0, -1, 0),
		coFloatx3(0, 0, -1)
	};

	const coFloatx3 t[] =
	{
		coFloatx3(0, -1, 0),
		coFloatx3(0, -1, 0),
		coFloatx3(-1, 0, 0),
		coFloatx3(0, 1, 0),
		coFloatx3(1, 0, 0),
		coFloatx3(0, 1, 0),
	};

	const coFloatx2 u[] = 
	{
		coFloatx2(0, 0),
		coFloatx2(0, 1),
		coFloatx2(1, 1),
		coFloatx2(1, 0),
	};
	
	const coFloatx3 positions[] = 
	{
		v[0], v[1], v[2], v[3],
		v[4], v[0], v[3], v[7],
		v[5], v[1], v[0], v[4],
		v[6], v[2], v[1], v[5],
		v[7], v[3], v[2], v[6],
		v[7], v[6], v[5], v[4]
	};
	coPushBackArray(p->positions, coArray<coFloatx3>(positions));
	const coFloatx3 normals[] = 
	{
		n[0], n[0], n[0], n[0],
		n[1], n[1], n[1], n[1],
		n[2], n[2], n[2], n[2],
		n[3], n[3], n[3], n[3],
		n[4], n[4], n[4], n[4],
		n[5], n[5], n[5], n[5]
	};
	coPushBackArray(p->normals, coArray<coFloatx3>(normals));
	const coFloatx3 tangents[] =
	{
		t[0], t[0], t[0], t[0],
		t[1], t[1], t[1], t[1],
		t[2], t[2], t[2], t[2],
		t[3], t[3], t[3], t[3],
		t[4], t[4], t[4], t[4],
		t[5], t[5], t[5], t[5]
	};
	coPushBackArray(p->tangents, coArray<coFloatx3>(tangents));
	const coFloatx2 uvs[] =
	{
		u[0], u[1], u[2], u[3],
		u[0], u[1], u[2], u[3],
		u[0], u[1], u[2], u[3],
		u[0], u[1], u[2], u[3],
		u[0], u[1], u[2], u[3],
		u[0], u[1], u[2], u[3]
	};
	coPushBackArray(p->uvs, coArray<coFloatx2>(uvs));
	const coUint16 indices[] = 
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	coPushBackArray(p->indexBuffer, coArray<const coUint8>(reinterpret_cast<const coUint8*>(indices), sizeof(indices)));
	p->indexSize8 = sizeof(indices[0]);
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
