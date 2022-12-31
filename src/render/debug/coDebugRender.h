// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <ecs/component/coComponent.h>
#include <ecs/entity/coEntityType.h>
class coRenderMesh;
class coNode;

class coDebugRender : public coComponent
{
	coDECLARE_COMPONENT(coDebugRender, coComponent);
public:
	coRenderMesh* GetRenderMesh() const { return renderMesh; }
	coNode* GetNode() const { return node; }
private:
	void Init(coEntity&);
	void Shutdown(coEntity&);
	void Start(coEntity&);
	void Stop(coEntity&);

	coRenderMesh* renderMesh = nullptr;
	coNode* node = nullptr;
};

coDECLARE_ENTITY(coDebugRenderEntity);