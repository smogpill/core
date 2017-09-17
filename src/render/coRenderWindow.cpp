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
#include "render/coRenderMaterial.h"
#include "render/coRenderer.h"
#include "render/coSwapChain.h"
#include "render/coShader.h"
#include "render/coRenderCommandBuffer.h"
#include "render/coRenderContext.h"
#include "render/coRenderEntity.h"
#include "render/coRenderMaterial.h"
#include "render/coRenderWorld.h"
#include "render/coRenderMesh.h"
#include "render/coRenderVertexChannels.h"
#include "pattern/scope/coDefer.h"
#include "lang/result/coResult_f.h"

coRenderWindow::coRenderWindow()
	: swapChain(nullptr)
	, renderFinishedSemaphore(nullptr)
	, surface(nullptr)
	, device(nullptr)
	, renderer(nullptr)
	, pipelineLayout(nullptr)
	, pipeline(nullptr)
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

	coTRY(InitSurface(config), "Failed to init the surface.");
	coTRY(SelectDevice(), "Failed to select device.");
	coTRY(InitSwapChain(config), "Failed to init the swap chain.");
	coTRY(InitSemaphores(), "Failed to init the semaphores.");
	coTRY(InitPipelineLayout(), "Failed to init the pipeline layout.");

	pipeline = coCreateRenderPipeline();

	return true;
}

coResult coRenderWindow::InitSurface(const InitConfig& _config)
{
	coRenderContext* renderContext = renderer->GetContext();
	coTRY(renderContext, nullptr);

	coSurface* p = coCreateSurface();
	coDEFER() { delete p; };
	coSurface::InitConfig c;
	c.rendererContext = renderContext;
	c.debugName = "RenderSurface";
#ifdef coMSWINDOWS
	c.hwnd = _config.hwnd;
#else
#	error "Not supported"
#endif
	coTRY(p->Init(c), "Failed to init the render surface.");
	coSwap(surface, p);
	return true;
}

coResult coRenderWindow::InitSwapChain(const InitConfig& _config)
{
	coSwapChain* p = coCreateSwapChain();
	coDEFER() { delete p; };
	coSwapChain::InitConfig c;
	c.device = device;
	c.size = _config.size;
	c.nbImages = 2;
	c.surface = surface;
	c.debugName = "SwapChain";
	coTRY(p->Init(c), "Failed to init the render swap chain.");
	coSwap(swapChain, p);
	return true;
}

coResult coRenderWindow::InitSemaphores()
{
	coRenderSemaphore* p = coCreateRenderSemaphore();
	coDEFER() { delete p; };
	coRenderSemaphore::InitConfig c;
	c.device = device;
	coTRY(p->Init(c), "Failed to init the render finished semaphore.");
	coSwap(renderFinishedSemaphore, p);
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

coResult coRenderWindow::Render(const coRenderWorld& _world)
{
	const coArray<coRenderEntity*>& entities = _world.GetEntities();
	coTRY(entities.count, nullptr);
	coRenderEntity* arbitraryEntity = entities[0];
	coTRY(arbitraryEntity, nullptr);
	const coRenderMesh* renderMesh = arbitraryEntity->GetRenderMesh();
	coTRY(renderMesh, nullptr);
	coRenderMaterial* material = renderMesh->GetMaterial();
	coTRY(material, nullptr);
	coTRY(InitPipeline(*pipeline, *material), "Failed to init the render pipeline.");
	coTRY(swapChain, nullptr);
	coTRY(swapChain->AcquireImage(), "Failed to acquire image for: " << *swapChain);
	coRenderSemaphore* imageAvailableSemaphore = swapChain->GetImageAvailableSemaphore();
	coTRY(imageAvailableSemaphore, nullptr);

	const coSwapChain::ImageInfo* imageInfo = swapChain->GetCurrentImageInfo();
	coTRY(imageInfo, nullptr);

	// Fill command buffer
	coRenderCommandBuffer* commandBuffer = imageInfo->commandBuffer;
	{
		coRenderer::FillConfig c;
		c.commandBuffer = commandBuffer;
		c.framebuffer = imageInfo->framebuffer;
		c.pipeline = pipeline;
		c.world = &_world;
		coTRY(renderer->FillCommandBuffer(c), "Failed to fill the command buffer.");
	}

	// Submit
	{
		coDynamicArray<coRenderSemaphore*> waitSemaphores;
		coDynamicArray<coRenderSemaphore*> finishSemaphores;
		coPushBack(waitSemaphores, imageAvailableSemaphore);
		coPushBack(finishSemaphores, renderFinishedSemaphore);
		coDynamicArray<coRenderCommandBuffer*> commandBuffers;
		coPushBack(commandBuffers, commandBuffer);
		coRenderDevice::SubmitConfig c;
		c.waitSemaphores = waitSemaphores;
		c.finishSemaphores = finishSemaphores;
		c.commandBuffers = commandBuffers;
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
	coRenderPipelineLayout* p = coCreateRenderPipelineLayout();
	coDEFER() { delete p; };
	coRenderPipelineLayout::InitConfig c;
	c.device = device;
	c.debugName = "WindowPipelineLayout";
	coTRY(p->Init(c), "Failed to init window pipeline layout.");
	coASSERT(!pipelineLayout);
	coSwap(pipelineLayout, p);
	return true;
}

coResult coRenderWindow::InitPipeline(coRenderPipeline& _out, const coRenderMaterial& _material)
{
	coRenderPipeline::InitConfig c;
	c.device = device;
	c.debugName = "WindowPipeline";
	c.renderPass = GetPass();
	c.layout = pipelineLayout;
	coDynamicArray<const coShader*> shaders;
	const coShader* vertexShader = _material.GetVertexShader();
	const coShader* fragmentShader = _material.GetFragmentShader();
	coTRY(vertexShader, nullptr);
	coTRY(fragmentShader, nullptr);
	coPushBack(shaders, vertexShader);
	coPushBack(shaders, fragmentShader);
	c.shaders = shaders;
	const coType* defaultVertexType = coRenderVertex_PosNormalTangentUv::GetStaticType();
	coDynamicArray<const coType*> channels;
	coPushBack(channels, defaultVertexType);
	c.vertexChannels = channels;
	const coInt32x2 swapChainSize = swapChain->GetSize();
	coDynamicArray<coFloatx2Rect> viewports;
	{
		coFloatx2Rect viewport;
		viewport.pos = coFloatx2(0.0f);
		viewport.size = coFloatx2(coFloat(swapChainSize.x), coFloat(swapChainSize.y));
		coPushBack(viewports, viewport);
	}
	c.viewports = viewports;
	coDynamicArray<coInt32x2Rect> scissors;
	{
		coInt32x2Rect scissor;
		scissor.pos = coInt32x2(0);
		scissor.size = swapChainSize;
		coPushBack(scissors, scissor);
	}
	c.scissors = scissors;
	coTRY(_out.Init(c), "Failed to init the pipeline.");
	return true;
}
