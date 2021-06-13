// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
#include "lang/result/coResult.h"
#include "pattern/singleton/coSingleton.h"

class coModule;

class coModuleManager
{
	coDECLARE_SINGLETON(coModuleManager);
public:
	coModuleManager();
	~coModuleManager();
	void Give(coModule& module);
	coResult Init();

private:
	coDynamicArray<coModule*> modules;
};
