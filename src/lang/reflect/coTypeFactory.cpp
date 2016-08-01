// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coTypeFactory.h"
#include "lang/reflect/coTypeBuilder.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"

coDEFINE_SINGLETON(coTypeFactory);

coResult coTypeFactory::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(InitTypes(), "Failed to init types.");
	return true;
}

coResult coTypeFactory::InitTypes()
{
	coReserve(types, typeBuilders.count);
	for (coTypeBuilder* builder : typeBuilders)
	{
		coASSERT(builder);
		coTRY(builder->InitType(), "Failed to init type.");
		const coType* type = builder->GetType();
		coTRY(type, nullptr);
		coPushBack(types, type);
	}
	for (coTypeBuilder* builder : typeBuilders)
	{
		coASSERT(builder);
		coTRY(builder->LinkType(), "Failed to link type.");
	}
	return true;
}

void coTypeFactory::Add(coTypeBuilder& _builder)
{
	coPushBack(typeBuilders, &_builder);
}
