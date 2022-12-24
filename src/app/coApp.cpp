// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"
#include "debug/log/coDefaultLogHandler.h"
#include "lang/result/coResult_f.h"
#include "render/manager/coRenderManager.h"
#include "io/dir/coDirectory_f.h"

coApp::~coApp()
{
	delete renderManager;
}

coResult coApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	coTRY(coInitDefaultDirs(), "Failed to init default dirs");

	//const InitConfig& config = static_cast<InitConfig>(_config);

	renderManager = new coRenderManager();
	coTRY(renderManager->Init(), nullptr);

	return true;
}
