// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderContext.h"
#include "lang/result/coResult_f.h"

coRenderContext::coRenderContext()
{
}

coRenderContext::~coRenderContext()
{
	coASSERT(devices.count == 0);
}

coResult coRenderContext::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	//const InitConfig& config = static_cast<const InitConfig&>(_config);

	return true;
}

coResult coRenderContext::Add(coSwapChain& _swapChain)
{
	coTRY(!coContains(swapChains, &_swapChain), nullptr);
	coTODO("Add thread safety");
	coPushBack(swapChains, &_swapChain);
	return true;
}
