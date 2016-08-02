// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderer.h"

coRuntimeApp::coRuntimeApp()
{

}

coRuntimeApp::~coRuntimeApp()
{
	delete renderer;
	renderer = nullptr;
}

coResult coRuntimeApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	renderer = new coRenderer();
	{
		coRenderer::InitConfig c;
		c.debugName = "Renderer";
		coTRY(renderer->Init(c), "Failed to init the renderer.");
	}
	return true;
}
