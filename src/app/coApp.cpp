// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"
#include "debug/log/coDefaultLogHandler.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeFactory.h"
#include "lang/reflect/coTypeRegistry.h"
#include "render/manager/coRenderManager.h"
#include "io/dir/coDirectory_f.h"

coApp::~coApp()
{
	delete renderManager;
	delete coTypeRegistry::instance;
	coTypeRegistry::instance = nullptr;
	delete coTypeFactory::instance;
	coTypeFactory::instance = nullptr;
	delete defaultLogHandler;
}

coResult coApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	//const InitConfig& config = static_cast<InitConfig>(_config);

	coTRY(coInitDefaultDirs(), nullptr);

	defaultLogHandler = new coDefaultLogHandler();
	coLogHandler::instance = defaultLogHandler;

	if (!coTypeFactory::instance)
		coTypeFactory::instance = new coTypeFactory();
	coTypeFactory::InitConfig c;
	coTRY(coTypeFactory::instance->Init(c), "Failed to init the type factory.");

	coTRY(!coTypeRegistry::instance, nullptr);
	coTypeRegistry::instance = new coTypeRegistry();

	renderManager = new coRenderManager();
	coTRY(renderManager->Init(), nullptr);

	return true;
}
