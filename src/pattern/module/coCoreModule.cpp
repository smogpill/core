// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coCoreModule.h"
#include <lang/result/coResult_f.h>
#include <lang/reflect/coTypeFactory.h>
#include "lang/reflect/coTypeRegistry.h"
#include "debug/log/coDefaultLogHandler.h"
#include "io/dir/coDirectory_f.h"
#include <pattern/ecs/coComponent.h>

coCoreModule::coCoreModule()
{
	coASSERT(coInitDefaultDirs());
	coLogHandler::SetInstance(new coDefaultLogHandler());
	coTypeFactory::SetInstance(new coTypeFactory());
	coTypeRegistry::SetInstance(new coTypeRegistry());
}

coCoreModule::~coCoreModule()
{
	coTypeRegistry::DestroyInstance();
	coTypeFactory::DestroyInstance();
	coLogHandler::DestroyInstance();
}

coResult coCoreModule::InitTypes()
{
	coTRY(Super::InitTypes(), nullptr);
	Add<coComponent>();
	return true;
}

coResult coCoreModule::Init()
{
	coTRY(Super::Init(), nullptr);
	coTRY(coTypeFactory::instance->Init(), nullptr);
	return true;
}
