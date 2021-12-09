// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "coComponent.h"
#include "../uuid/coUuid_f.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeFactory.h>
#include <lang/reflect/coField_f.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/pack/coPackEntry_f.h>

coEntity::coEntity()
{
	static coUint64 uuidGenerator = 0;
	uuid.low = ++uuidGenerator;
}

coEntity::coEntity(const coEntity& other)
{
	for (const coComponent* comp : other.components)
	{
		coPushBack(components, comp->Clone());
		//newComp->entity = this;
	}
}

coEntity::~coEntity()
{
	for (coComponent* comp : components)
		delete comp;
}

void coEntity::AddAndGiveOwnership(coComponent& comp)
{
	comp.entity = this;
	coPushBack(components, &comp);
}

void coEntity::Write(coBinaryOutputStream& stream) const
{
	stream << uuid;
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
	stream >> uuid;
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
coResult coEntity::Init()
{
	for (coComponent* comp : components)
	{
		coTRY(comp->Init(), nullptr);
	}
	return true;
}

void coEntity::Release()
{
	for (coComponent* comp : components)
	{
		comp->Release();
	}
}

coResult coEntity::Start()
{
	for (coComponent* comp : components)
	{
		coTRY(comp->Start(), nullptr);
	}
	return true;
}

void coEntity::Stop()
{
	for (coComponent* comp : components)
	{
		comp->Stop();
	}
}

coResult coEntity::Save(coBinaryOutputStream& stream) const
{
	for (coComponent* comp : components)
	{
		const coType* type = comp->GetType();
		comp->Write(stream);
	}
	return true;
}

coResult coEntity::Load(coBinaryInputStream& stream)
{
	coASSERT(false);
	return true;
}

void coEntity::Save(coPackEntry& entry) const
{
	for (coComponent* comp : components)
	{
		const coType* type = comp->GetType();
		if (type->packFormat)
		{
			coPackEntry compEntry = entry.PushEntry(*type->packFormat);
			for (const coField* field : type->fields)
			{
				//compEntry.SetField(field->name, );
			}
		}
	}
}

coEntity* coEntity::Clone() const
{
	//coEntity* clone = new coEntity(*this);
	coASSERT(false);
	return nullptr;
}
