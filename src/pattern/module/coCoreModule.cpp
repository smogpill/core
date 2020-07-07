// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coCoreModule.h"
#include <lang/result/coResult_f.h>
#include <lang/reflect/coTypeFactory.h>

#include <pattern/ecs/coComponent.h>

coCoreModule::coCoreModule()
{
	coASSERT(!coTypeFactory::instance);
	coTypeFactory::instance = new coTypeFactory();
}

coCoreModule::~coCoreModule()
{
	delete coTypeFactory::instance;
	coTypeFactory::instance = nullptr;
}

coResult coCoreModule::OnInitTypes()
{
	Add<coComponent>();
	return true;
}
