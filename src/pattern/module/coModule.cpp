// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coModule.h"
#include <lang/reflect/coTypeFactory.h>
#include <lang/reflect/coType.h>
#include <lang/result/coResult_f.h>

coResult coModule::Init()
{
	coTRY(OnInitTypes(), nullptr);
	return true;
}

void coModule::Add(const coType& type)
{
	coASSERT(type.builder);
	coTypeFactory::instance->Add(*type.builder);
}
