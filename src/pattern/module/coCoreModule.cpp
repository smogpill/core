// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coCoreModule.h"
#include <lang/result/coResult_f.h>
#include "lang/reflect/coTypeRegistry.h"
#include "lang/reflect/coTypeAutoRegistrator.h"
#include "debug/log/coDefaultLogHandler.h"
#include "io/dir/coDirectory_f.h"
#include <pattern/ecs/coComponent.h>

coCoreModule::coCoreModule()
{
	coASSERT(coInitDefaultDirs());
	coLogHandler::SetInstance(new coDefaultLogHandler());
}

coCoreModule::~coCoreModule()
{
	coLogHandler::DestroyInstance();
}

coResult coCoreModule::Init()
{
	coTRY(Super::Init(), nullptr);
	coTRY(coTypeAutoRegistrator::RegisterAll(), nullptr);
	return true;
}
