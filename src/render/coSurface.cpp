// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coSurface.h"
#include "lang/result/coResult_f.h"

coSurface::coSurface()
	: renderer(nullptr)
{
}

coSurface::InitConfig::InitConfig()
	: renderer(nullptr)
#ifdef coMSWINDOWS
	, hwnd(NULL)
#endif
{
}

coResult coSurface::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	renderer = config.renderer;
	coTRY(renderer, nullptr);
	return true;
}
