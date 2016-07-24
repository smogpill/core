// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"
#include "debug/log/coDefaultLogHandler.h"
#include "lang/result/coResult_f.h"

coApp::coApp()
	: defaultLogHandler(nullptr)
{

}

coApp::~coApp()
{
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

	return true;
}
