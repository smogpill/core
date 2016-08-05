// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "render/vulkan/coResult_f_vk.h"
#include "render/vulkan/coRenderContext_vk.h"
#include "lang/result/coResult_f.h"

void coRenderer::OnImplConstruct()
{
	coASSERT(impl == nullptr);
	impl = new coRenderContext_vk();
}

void coRenderer::OnImplDestruct()
{
	coRenderContext_vk* renderContext_vk = static_cast<coRenderContext_vk*>(impl);
	delete renderContext_vk;
	renderContext_vk = nullptr;
}

coResult coRenderer::OnImplInit(const InitConfig& /*_config*/)
{
	coRenderContext_vk* renderContext_vk = static_cast<coRenderContext_vk*>(impl);

	coRenderContext_vk::InitConfig c;
	coTRY(renderContext_vk->Init(c), "Failed to init the render context.");
	return true;
}
