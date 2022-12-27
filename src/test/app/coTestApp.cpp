// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test/pch.h"
#include "coTestApp.h"
#include <debug/log/coDefaultLogHandler.h>
#include "../unit/coTestRegistry.h"

coTestApp::coTestApp()
{
	logHandler = new coDefaultLogHandler();
}

coTestApp::~coTestApp()
{
	delete logHandler;
}

coResult coTestApp::Run()
{
	coTestRegistry* testRegistry = coTestRegistry::instance;
	if (testRegistry)
	{
		testRegistry->RunAllTests();
	}
	return true;
}
