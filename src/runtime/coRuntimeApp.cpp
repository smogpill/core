// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderer.h"
#include "render/coSwapChain.h"
#include "render/coSurface.h"
#include "render/coRenderFactory.h"

coRuntimeApp::coRuntimeApp()
	: renderer(nullptr)
	, window(nullptr)
	, swapChain(nullptr)
	, surface(nullptr)
{

}

coRuntimeApp::~coRuntimeApp()
{
	delete swapChain;
	swapChain = nullptr;
	delete surface;
	surface = nullptr;
	delete window;
	window = nullptr;
	delete renderer;
	renderer = nullptr;
}

coResult coRuntimeApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	renderer = coCreateRenderer();
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

	surface = coCreateSurface();
	{
		coSurface::InitConfig c;
		c.renderer = renderer;
		c.debugName = "MainRenderSurface";
#ifdef coMSWINDOWS
		c.hwnd = static_cast<HWND>(window->GetImpl());
#else
#	error "Not supported"
#endif
		coTRY(surface->Init(c), "Failed to init the main render surface.");
	}

	swapChain = coCreateSwapChain();
	{
		coSwapChain::InitConfig c;
		c.size = coInt32x2(800, 600);
		c.nbImages = 2;
		c.surface = surface;
		c.debugName = "MainSwapChain";
		coTRY(swapChain->Init(c), "Failed to init the main render sswap chain.");
	}

	return true;
}
