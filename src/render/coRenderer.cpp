// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/coRenderContext.h"
#include "render/coSwapChain.h"
#include "render/coSurface.h"
#include "render/coRenderFactory.h"
#include "render/coRenderDevice.h"
#include "render/coRenderSemaphore.h"
#include "lang/result/coResult_f.h"

coRenderer::coRenderer()
	: context(nullptr)
{

}

coRenderer::~coRenderer()
{
	for (coRenderDevice* device : devices)
	{
		coASSERT(device);
		device->Stop();
	}
	delete context;
	context = nullptr;
}

coRenderer::FillConfig::FillConfig()
	: framebuffer(nullptr)
	, commandBuffer(nullptr)
	, pipeline(nullptr)
	, world(nullptr)
{

}

coResult coRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	context = coCreateRenderContext();
	{
		coRenderContext::InitConfig c;
		c.debugName = "RenderContext";
		coTRY(context->Init(c), "Failed to init the render context.");
	}

	coTRY(InitDevices(), "Failed to init devices.");
	return true;
}

coResult coRenderer::InitDevices()
{
	coASSERT(!devices.count);
	for (coRenderDevice* device : context->GetDevices())
	{
		coBool b;
		if (device->SupportsGraphics(b) && b)
		{
			if (device->Start())
			{
				coPushBack(devices, device);
			}
		}
	}
	return true;
}

coResult coRenderer::FillCommandBuffer(const FillConfig& /*_config*/)
{
	return true;
}
