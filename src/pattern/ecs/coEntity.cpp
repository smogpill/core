// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "coComponent.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeFactory.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>

coEntity::~coEntity()
{
	for (coComponent* comp : components)
		delete comp;
}

void coEntity::AddAndGiveOwnership(coComponent& comp)
{
	coPushBack(components, &comp);
}

void coEntity::Write(coBinaryOutputStream& stream) const
{
	stream << components.count;
	for (coComponent* comp : components)
	{
		const coType* type = comp->GetType();
		stream << type->nameHash;
		stream << *comp;
	}
}

void coEntity::Read(coBinaryInputStream& stream)
{
	coUint32 nbComponents;
	stream >> nbComponents;
	coReserve(components, nbComponents);
	for (coUint i = 0; i < nbComponents; ++i)
	{
		coUint32 nameHash;
		stream >> nameHash;
		const coType* type = coTypeFactory::instance->Get(nameHash);
		coASSERT(type);
		coComponent* comp = static_cast<coComponent*>(type->createFunc());
		stream >> *comp;
		AddAndGiveOwnership(*comp);
	}
}
