// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coTypeFactory.h"
#include "lang/reflect/coTypeBuilder.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coScopeExit.h"

coDEFINE_SINGLETON(coTypeFactory);

coTypeFactory::~coTypeFactory()
{
	coClear(types);
	for (coTypeBuilder* builder : typeBuilders)
	{
		coASSERT(builder);
		builder->Clear();
	}
}

coResult coTypeFactory::OnInit()
{
	coTRY(Super::OnInit(), nullptr);
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

const coType* coTypeFactory::Get(coUint32 hash) const
{
	for (const coType* e : types)
	{
		if (e->nameHash == hash)
			return e;
	}
	return nullptr;
}
