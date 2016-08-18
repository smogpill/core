// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coSwapChain.h"
#include "lang/result/coResult_f.h"

coSwapChain::coSwapChain()
	: surface(nullptr)
	, imageAvailableSemaphore(nullptr)
	, renderPass(nullptr)
{
}

coSwapChain::InitConfig::InitConfig()
	: surface(nullptr)
	, oldSwapChain(nullptr)
	, size(0)
	, nbImages(2)
{

}

coSwapChain::~coSwapChain()
{
	coASSERT(!imageViews.count);
	coASSERT(!images.count);
	coASSERT(!imageAvailableSemaphore);
	coASSERT(!renderPass);
	coASSERT(!frameBuffers.count);
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