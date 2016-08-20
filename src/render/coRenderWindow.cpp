// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderWindow.h"
#include "render/coRenderFactory.h"
#include "render/coRenderDevice.h"
#include "render/coSurface.h"
#include "render/coRenderSemaphore.h"
#include "render/coSwapChain.h"
#include "render/coRenderContext.h"
#include "lang/result/coResult_f.h"

coRenderWindow::coRenderWindow()
	: swapChain(nullptr)
	, renderFinishedSemaphore(nullptr)
	, surface(nullptr)
	, device(nullptr)
{

}

coRenderWindow::~coRenderWindow()
{
	if (device)
	{
		coCHECK(device->WaitForIdle(), nullptr);
	}
	delete renderFinishedSemaphore;
	renderFinishedSemaphore = nullptr;
	delete swapChain;
	swapChain = nullptr;
	delete surface;
	surface = nullptr;
}

coRenderWindow::InitConfig::InitConfig()
	: context(nullptr)
	, size(0)
#ifdef coMSWINDOWS
	, hwnd(NULL)
#endif
{
}

coResult coRenderWindow::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coASSERT(!device);
	coTRY(config.size.x >= 0, nullptr);
	coTRY(config.size.y >= 0, nullptr);

	surface = coCreateSurface();
	{
		coSurface::InitConfig c;
		c.rendererContext = config.context;
		c.debugName = "RenderSurface";
#ifdef coMSWINDOWS
		c.hwnd = config.hwnd;
#else
#	error "Not supported"
#endif
		coTRY(surface->Init(c), "Failed to init the render surface.");
	}

	coTRY(SelectDevice(config), "Failed to select device.");

	swapChain = coCreateSwapChain();
	{
		coSwapChain::InitConfig c;
		c.device = device;
		c.size = config.size;
		c.nbImages = 2;
		c.surface = surface;
		c.debugName = "SwapChain";
		coTRY(swapChain->Init(c), "Failed to init the render swap chain.");
	}

	{
		coASSERT(!renderFinishedSemaphore);
		renderFinishedSemaphore = coCreateRenderSemaphore();
		coRenderSemaphore::InitConfig c;
		c.device = device;
		coTRY(renderFinishedSemaphore->Init(c), "Failed to init the render finished semaphore.");
	}

	return true;
}

coResult coRenderWindow::SelectDevice(const InitConfig& config)
{
	device = nullptr;
	for (coRenderDevice* d : config.context->GetDevices())
	{
		coASSERT(d);
		if (!d->IsStarted())
			continue;

		const coRenderDevice::DeviceType deviceType = d->GetDeviceType();
		if (deviceType != coRenderDevice::discreteGpu && deviceType != coRenderDevice::integratedGpu)
			continue;
		coBool b;
		coCHECK(d->SupportsGraphics(b), nullptr);
		if (!b)
			continue;

		coCHECK(d->SupportsSurface(b, *surface), nullptr);
		if (!b)
			continue;

		if (!device || device->GetDeviceType() == coRenderDevice::integratedGpu)
		{
			device = d;
		}
	}
	return true;
}

coResult coRenderWindow::Render()
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
		coTRY(device->Submit(c), nullptr);
	}

	// Present
	{
		coDynamicArray<coRenderSemaphore*> waitSemaphores;
		coPushBack(waitSemaphores, renderFinishedSemaphore);
		coTRY(swapChain->Present(waitSemaphores), "Failed to present: " << *swapChain);
	}
	return true;
}