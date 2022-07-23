// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/module/coModuleManager.h"
#include "pattern/module/coModule.h"
#include "pattern/module/coCoreModule.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/result/coResult_f.h"

coDEFINE_SINGLETON(coModuleManager);

coModuleManager::coModuleManager()
{
	Give(*new coCoreModule());
}

coModuleManager::~coModuleManager()
{
	for (coModule* p : modules)
		delete p;
}

void coModuleManager::Give(coModule& module)
{
	coPushBack(modules, &module);
}

coResult coModuleManager::Init()
{
	for (coModule* module : modules)
		coTRY(module->Init(), nullptr);
	return true;
}
