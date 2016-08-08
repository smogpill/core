// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderer.h"

coRuntimeApp::coRuntimeApp()
	: renderer(nullptr)
	, window(nullptr)
{

}

coRuntimeApp::~coRuntimeApp()
{
	delete window;
	window = nullptr;
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

	window = new coWindow();
	{
		coWindow::InitConfig c;
		c.debugName = "Window";
		c.name = "Runtime";
		c.clientSize = coInt32x2(800, 600);
		coTRY(window->Init(c), "Failed to init the window.");
		coTRY(window->SetShowState(coWindow::ShowState::default), nullptr);
	}



	return true;
}
