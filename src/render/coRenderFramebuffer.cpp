// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderFramebuffer.h"
#include "lang/result/coResult_f.h"

coRenderFramebuffer::coRenderFramebuffer()
	: size(0)
{

}

coRenderFramebuffer::InitConfig::InitConfig()
	: size(0)
	, pass(nullptr)
{

}

coResult coRenderFramebuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	size = config.size;
	coTRY(size.x >= 0, nullptr);
	coTRY(size.y >= 0, nullptr);
	return true;
}
