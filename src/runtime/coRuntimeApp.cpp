// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "runtime/coTestScene.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderer.h"
#include "render/coRenderWindow.h"
#include "render/coRenderWorld.h"
#include "render/coRenderFactory.h"
#include "container/array/coDynamicArray_f.h"
#include "pattern/scope/coDefer.h"
#include "gui/immediate/coImGui.h"

coRuntimeApp::coRuntimeApp()
	: window(nullptr)
	, renderer(nullptr)
	, renderWindow(nullptr)
	, testScene(nullptr)
	, renderWorld(nullptr)
	, imGui(nullptr)
{

}

coRuntimeApp::~coRuntimeApp()
{
	delete testScene;
	delete imGui;
	delete renderWorld;
	delete renderWindow;
	delete window;
	delete renderer;
}

coResult coRuntimeApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	coTRY(InitRenderer(), "Failed to init the renderer.");
	coTRY(InitWindow(), "Failed to init the window.");
	coTRY(InitRenderWindow(), "Failed to init the render window.");
	coTRY(InitRenderWorld(), "Failed to init the render world.");
	coTRY(InitImGui(), "Failed to init the imGui.");
	coTRY(InitTestScene(), "Failed to init the test scene.");

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

		coCHECK(Update(), "Failed to render.");
	}
	return true;
}

coResult coRuntimeApp::Update()
{
	if (testScene)
	{
		coCHECK(testScene->Update(), "Failed to update.");
	}
	
	if (renderWindow)
	{
		coTRY(renderWorld, nullptr);
		coTRY(renderWindow->Render(*renderWorld), "Failed to render the render window: " << *renderWindow);
	}
	return true;
}

coResult coRuntimeApp::InitRenderer()
{
	coRenderer* p = coCreateRenderer();
	coDEFER() { delete p; };
	coRenderer::InitConfig c;
	c.debugName = "MainRenderer";
	coTRY(p->Init(c), "Failed to init the renderer.");
	coSwap(renderer, p);
	return true;
}

coResult coRuntimeApp::InitTestScene()
{
	coTestScene* p = new coTestScene();
	coDEFER() { delete p; };
	coTestScene::InitConfig c;
	c.debugName = "TestScene";
	c.renderDevice = renderWindow->GetDevice();
	coTRY(p->Init(c), "Failed to init the test scene.");
	coTRY(renderWorld, nullptr);
	for (coRenderEntity* entity : p->GetEntities())
	{
		coTRY(renderWorld->Add(*entity), nullptr);
	}
	coSwap(testScene, p);
	return true;
}

coResult coRuntimeApp::InitRenderWorld()
{
	coRenderWorld* p = new coRenderWorld();
	coDEFER() { delete p; };
	coRenderWorld::InitConfig c;
	c.debugName = "TestRenderWorld";
	coTRY(p->Init(c), "Failed to init the render world.");
	coSwap(renderWorld, p);
	return true;
}

coResult coRuntimeApp::InitImGui()
{
	coImGui* p = new coImGui();
	coDEFER() { delete p; };
	coRenderDevice* device = renderer->GetMainDevice();
	coTRY(device, nullptr);
	p->SetRenderDevice(*device);
	coTRY(p->Init(), "Failed to init the imgui.");
	coSwap(imGui, p);
	return true;
}

coResult coRuntimeApp::InitWindow()
{
	coWindow* p = new coWindow();
	coDEFER() { delete p; };
	coWindow::InitConfig c;
	c.debugName = "Window";
	c.name = "Runtime";
	c.clientSize = coInt32x2(800, 600);
	coTRY(p->Init(c), "Failed to init the window.");
	coTRY(p->SetShowState(coWindow::ShowState::default), nullptr);
	coSwap(window, p);
	return true;
}

coResult coRuntimeApp::InitRenderWindow()
{
	coRenderWindow* p = new coRenderWindow();
	coDEFER() { delete p; };
	coRenderWindow::InitConfig c;
	c.renderer = renderer;
	c.debugName = "MainRenderWindow";
	c.size = window->GetClientSize();
#ifdef coMSWINDOWS
	c.hwnd = static_cast<HWND>(window->GetImpl());;
#endif
	coTRY(p->Init(c), "Failed to init the render window.");
	coSwap(renderWindow, p);
	return true;
}