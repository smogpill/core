// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderWindow.h"
#include "render/coRenderFactory.h"
#include "render/coRenderDevice.h"
#include "render/coSurface.h"
#include "render/coRenderSemaphore.h"
#include "render/coRenderPipeline.h"
#include "render/coRenderPipelineLayout.h"
#include "render/coRenderer.h"
#include "render/coSwapChain.h"
#include "render/coRenderCommandBuffer.h"
#include "render/coRenderContext.h"
#include "render/coRenderVertexChannels.h"
#include "pattern/scope/coDefer.h"
#include "lang/result/coResult_f.h"

coRenderWindow::coRenderWindow()
	: swapChain(nullptr)
	, renderFinishedSemaphore(nullptr)
	, surface(nullptr)
	, device(nullptr)
	, renderer(nullptr)
	, pipeline(nullptr)
	, pipelineLayout(nullptr)
	, vertexShader(nullptr)
	, fragmentShader(nullptr)
{

}

coRenderWindow::~coRenderWindow()
{
	if (device)
	{
		coCHECK(device->WaitForIdle(), nullptr);
	}
	delete pipeline;
	delete pipelineLayout;
	delete renderFinishedSemaphore;
	delete swapChain;
	delete surface;
}

coRenderWindow::InitConfig::InitConfig()
	: renderer(nullptr)
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
	renderer = config.renderer;
	coTRY(renderer, nullptr);
	coASSERT(!device);
	coTRY(config.size.x >= 0, nullptr);
	coTRY(config.size.y >= 0, nullptr);

	coRenderContext* renderContext = renderer->GetContext();
	coTRY(renderContext, nullptr);

	surface = coCreateSurface();
	{
		coSurface::InitConfig c;
		c.rendererContext = renderContext;
		c.debugName = "RenderSurface";
#ifdef coMSWINDOWS
		c.hwnd = config.hwnd;
#else
#	error "Not supported"
#endif
		coTRY(surface->Init(c), "Failed to init the render surface.");
	}

	coTRY(SelectDevice(), "Failed to select device.");

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

	coTRY(InitPipelineLayout(), "Failed to init the pipeline layout.");
	coTRY(InitPipeline(), "Failed to init the pipeline.");

	return true;
}

coResult coRenderWindow::SelectDevice()
{
	device = nullptr;
	coRenderContext* context = GetContext();
	for (coRenderDevice* d : context->GetDevices())
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

	coTRY(swapChain->AcquireImage(), "Failed to acquire new swap chain image.");

	const coSwapChain::ImageInfo* imageInfo = swapChain->GetCurrentImageInfo();
	coTRY(imageInfo, nullptr);

	// Fill command buffer
	coRenderCommandBuffer* commandBuffer = imageInfo->commandBuffer;
	{
		coRenderer::FillConfig c;
		c.commandBuffer = commandBuffer;
		c.framebuffer = imageInfo->framebuffer;
		coTRY(renderer->FillCommandBuffer(c), "Failed to fill the command buffer.");
	}

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

coRenderContext* coRenderWindow::GetContext() const
{
	return renderer ? renderer->GetContext() : nullptr;
}

coRenderPass* coRenderWindow::GetPass() const
{
	return swapChain ? swapChain->GetPass() : nullptr;
}

coResult coRenderWindow::InitPipelineLayout()
{
	coASSERT(!pipelineLayout);
	coRenderPipelineLayout* pl = coCreateRenderPipelineLayout();
	coDEFER() { delete pl; };
	coRenderPipelineLayout::InitConfig c;
	c.device = device;
	c.debugName = "WindowPipelineLayout";
	coTRY(pl->Init(c), "Failed to init window pipeline layout.");
	coASSERT(!pipelineLayout);
	coSwap(pipelineLayout, pl);
	return true;
}

coResult coRenderWindow::InitPipeline()
{
	coRenderPipeline* p = coCreateRenderPipeline();
	coDEFER() { delete p; };
	coRenderPipeline::InitConfig c;
	c.device = device;
	c.debugName = "WindowPipeline";
	c.renderPass = GetPass();
	c.layout = pipelineLayout;
	coDynamicArray<const coShader*> shaders;
	coPushBack(shaders, vertexShader);
	coPushBack(shaders, fragmentShader);
	c.shaders = shaders;
	const coType* defaultVertexType = coRenderVertex_PosNormalUv::GetStaticType();
	coDynamicArray<const coType*> channels;
	coPushBack(channels, defaultVertexType);
	c.vertexChannels = channels;
	coDynamicArray<coFloatx2Rect> viewports;
	coFloatx2Rect viewport;
	const coInt32x2 swapChainSize = swapChain->GetSize();
	viewport.pos = coFloatx2(0.0f);
	viewport.size = coFloatx2(coFloat(swapChainSize.x), coFloat(swapChainSize.y));
	coPushBack(viewports, viewport);
	c.viewports = viewports;
	coTRY(p, "Failed to init the pipeline.");
	coASSERT(!pipeline);
	coSwap(pipeline, p);
	return true;
}
