// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"
#include "debug/log/coDefaultLogHandler.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeFactory.h"
#include "lang/reflect/coTypeRegistry.h"

coApp::coApp()
	: defaultLogHandler(nullptr)
{

}

coApp::~coApp()
{
	delete coTypeRegistry::instance;
	coTypeRegistry::instance = nullptr;
	delete coTypeFactory::instance;
	coTypeFactory::instance = nullptr;
	delete defaultLogHandler;
	defaultLogHandler = nullptr;
}

coApp::InitConfig::InitConfig()
	: argv(nullptr)
	, nbArgs(0)
{

}

coResult coApp::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	defaultLogHandler = new coDefaultLogHandler();
	coLogHandler::instance = defaultLogHandler;

	if (!coTypeFactory::instance)
	{
		coTypeFactory::instance = new coTypeFactory();
		coTypeFactory* typeFactory = coTypeFactory::instance;
		coTypeFactory::InitConfig c;
		coTRY(typeFactory->Init(c), "Failed to init the type factory.");
	}

	coTRY(!coTypeRegistry::instance, nullptr);
	coTypeRegistry::instance = new coTypeRegistry();

	return true;
}
