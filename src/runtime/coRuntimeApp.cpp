// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "runtime/pch.h"
#include "runtime/coRuntimeApp.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "render/coRenderContext.h"
#include "render/coSwapChain.h"
#include "render/coSurface.h"
#include "render/coRenderFactory.h"
#include "render/coRenderDevice.h"
#include "render/coRenderSemaphore.h"
#include "container/array/coDynamicArray_f.h"

coRuntimeApp::coRuntimeApp()
	: renderer(nullptr)
	, window(nullptr)
	, swapChain(nullptr)
	, surface(nullptr)
	, renderDevice(nullptr)
	, renderFinishedSemaphore(nullptr)
{

}

coRuntimeApp::~coRuntimeApp()
{
	if (renderDevice)
	{
		coCHECK(renderDevice->WaitForIdle(), nullptr);
	}
	delete renderFinishedSemaphore;
	renderFinishedSemaphore = nullptr;
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
		coRenderContext::InitConfig c;
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

	coTRY(SelectRenderDevice(), "Failed to select a render device.");
	coTRY(renderDevice, "No render device available.");
	coTRY(renderDevice->Start(), "Failed to start the render device.");
	
	swapChain = coCreateSwapChain();
	{
		coSwapChain::InitConfig c;
		c.device = renderDevice;
		c.size = coInt32x2(800, 600);
		c.nbImages = 2;
		c.surface = surface;
		c.debugName = "MainSwapChain";
		coTRY(swapChain->Init(c), "Failed to init the main render sswap chain.");
	}

	{
		coASSERT(!renderFinishedSemaphore);
		renderFinishedSemaphore = coCreateRenderSemaphore();
		coRenderSemaphore::InitConfig c;
		c.device = renderDevice;
		coTRY(renderFinishedSemaphore->Init(c), "Failed to init the render finished semaphore.");
	}

	return true;
}

coResult coRuntimeApp::SelectRenderDevice()
{
	renderDevice = nullptr;
	for (coRenderDevice* device : renderer->GetDevices())
	{
		coASSERT(device);
		const coRenderDevice::DeviceType deviceType = device->GetDeviceType();
		if (deviceType != coRenderDevice::discreteGpu && deviceType != coRenderDevice::integratedGpu)
			continue;
		coBool b;
		coCHECK(device->SupportsGraphics(b), nullptr);
		if (!b)
			continue;

		coCHECK(device->SupportsSurface(b, *surface), nullptr);
		if (!b)
			continue;

		if (!renderDevice || renderDevice->GetDeviceType() == coRenderDevice::integratedGpu)
		{
			renderDevice = device;
		}
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
	coTRY(swapChain, nullptr);
	coTRY(swapChain->AcquireImage(), "Failed to acquire image for: " << *swapChain);
	coRenderSemaphore* imageAvailableSemaphore = swapChain->GetImageAvailableSemaphore();
	coTRY(imageAvailableSemaphore, nullptr);

	// Submit
	{
		coDynamicArray<coRenderSemaphore*> waitSemaphores;
		coDynamicArray<coRenderSemaphore*> finishSemaphores;
		coPushBack(waitSemaphores, imageAvailableSemaphore);
		coPushBack(finishSemaphores, renderFinishedSemaphore);
		coRenderDevice::SubmitConfig c;
		c.waitSemaphores = waitSemaphores;
		c.finishSemaphores = finishSemaphores;
		coTRY(renderDevice->Submit(c), nullptr);
	}

	// Present
	{
		coDynamicArray<coRenderSemaphore*> waitSemaphores;
		coPushBack(waitSemaphores, renderFinishedSemaphore);
		coTRY(swapChain->Present(waitSemaphores), "Failed to present: " << *swapChain);
	}
	return true;
}