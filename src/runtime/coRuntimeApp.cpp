// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderer.h"
#include "render/coRenderWindow.h"
#include "container/array/coDynamicArray_f.h"

coRuntimeApp::coRuntimeApp()
	: window(nullptr)
	, renderer(nullptr)
	, renderWindow(nullptr)
{

}

coRuntimeApp::~coRuntimeApp()
{
	delete renderWindow;
	renderWindow = nullptr;
	delete window;
	window = nullptr;
	delete renderer;
	renderer = nullptr;
}

coResult coRuntimeApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	coTRY(!renderer, nullptr);
	renderer = new coRenderer();
	{
		coRenderer::InitConfig c;
		c.debugName = "MainRenderer";
		coTRY(renderer->Init(c), "Failed to init the renderer.");
	}

	coTRY(!window, nullptr);
	window = new coWindow();
	{
		coWindow::InitConfig c;
		c.debugName = "Window";
		c.name = "Runtime";
		c.clientSize = coInt32x2(800, 600);
		coTRY(window->Init(c), "Failed to init the window.");
		coTRY(window->SetShowState(coWindow::ShowState::default), nullptr);
	}

	coTRY(!renderWindow, nullptr);
	renderWindow = new coRenderWindow();
	{
		coRenderWindow::InitConfig c;
		c.renderer = renderer;
		c.debugName = "MainRenderWindow";
		c.size = window->GetClientSize();
#ifdef coMSWINDOWS
		c.hwnd = static_cast<HWND>(window->GetImpl());;
#endif
		coTRY(renderWindow->Init(c), "Failed to init the render window.");
	}

	return true;
}

coResult coRuntimeApp::OnStart()
{
	coTRY(Super::OnStart(), nullptr);
	while (!exitRequested)
	{
		coTRY(ProcessEvents(), "Failed to process events.");
		if (exitRequested)
			break;

		coCHECK(Render(), "Failed to render");
	}
	return true;
}

coResult coRuntimeApp::Render()
{
	if (renderWindow)
	{
		coTRY(renderWindow->Render(), "Failed to render the render window: " << *renderWindow);
	}
	return true;
}
