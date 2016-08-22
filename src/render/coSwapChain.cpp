// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coSwapChain.h"
#include "render/coRenderCommandBuffer.h"
#include "render/coRenderFramebuffer.h"
#include "render/coRenderImageView.h"
#include "render/coRenderImage.h"
#include "lang/result/coResult_f.h"

coSwapChain::coSwapChain()
	: surface(nullptr)
	, imageAvailableSemaphore(nullptr)
	, pass(nullptr)
	, currentImageIndex(-1)
	, size(0)
{
}

coSwapChain::~coSwapChain()
{
	coASSERT(!imageAvailableSemaphore);
	coASSERT(!pass);
	coASSERT(!imageInfos.count);
}

coSwapChain::InitConfig::InitConfig()
	: surface(nullptr)
	, oldSwapChain(nullptr)
	, size(0)
	, nbImages(2)
{

}

coSwapChain::ImageInfo::ImageInfo()
	: image(nullptr)
	, imageView(nullptr)
	, framebuffer(nullptr)
	, commandBuffer(nullptr)
{

}

coSwapChain::ImageInfo::~ImageInfo()
{
	delete commandBuffer;
	delete framebuffer;
	delete imageView;
	delete image;
}

coResult coSwapChain::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	surface = config.surface;
	coTRY(surface, nullptr);
	return true;
}

coResult coSwapChain::AcquireImage()
{
	return true;
}

coResult coSwapChain::Present(const coArray<coRenderSemaphore*> /*_waitSemaphores*/)
{
	return true;
}