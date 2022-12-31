// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coDebugRender.h"
#include <ecs/component/coComponent_f.h>
#include <ecs/lib/node/coNode.h>
#include <ecs/entity/coEntity_f.h>
#include "../mesh/coRenderMesh.h"
#include "coDebugRenderer.h"

coBEGIN_COMPONENT(coDebugRender);
component->AddDependency<coNode>();
coDEFINE_COMPONENT_INIT();
coDEFINE_COMPONENT_SHUTDOWN();
coDEFINE_COMPONENT_START();
coDEFINE_COMPONENT_STOP();
coEND_COMPONENT();

coBEGIN_ENTITY(coDebugRenderEntity);
entity->AddComponent<coDebugRender>();
coEND_ENTITY();

void coDebugRender::Init(coEntity& entity)
{
	Base::Init(entity);
	node = entity.GetComponent<coNode>();
	renderMesh = new coRenderMesh();
}

void coDebugRender::Shutdown(coEntity& entity)
{
	delete renderMesh; renderMesh = nullptr;
	Base::Shutdown(entity);
}

void coDebugRender::Start(coEntity& entity)
{
	Base::Start(entity);
	coDebugRenderer* renderer = coDebugRenderer::instance;
	if (renderer)
		renderer->Add(*this);
}

void coDebugRender::Stop(coEntity& entity)
{
	coDebugRenderer* renderer = coDebugRenderer::instance;
	if (renderer)
		renderer->Remove(*this);
	Base::Stop(entity);
}
