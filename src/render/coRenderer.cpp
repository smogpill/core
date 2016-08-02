// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderer.h"
#include "lang/result/coResult_f.h"

coRenderer::coRenderer()
	: impl(nullptr)
{
	OnImplConstruct();
}

coRenderer::~coRenderer()
{
	OnImplDestruct();
}

coResult coRenderer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	coTRY(OnImplInit(config), "Implementation failed to initialize");

	return true;
}
