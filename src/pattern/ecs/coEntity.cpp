// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "coComponent.h"
#include "../uuid/coUuid_f.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeFactory.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <container/array/coFixedArray_f.h>
#include "component/coComponentIterator.h"

coEntity::coEntity()
{
	static coUint64 uuidGenerator = 0;
	uuid.low = ++uuidGenerator;
}

coEntity::coEntity(const coEntity& other)
{
	coASSERT(false);
	/*
	for (coComponent* comp : coLinkedComponents(&other))
	{
		coPushBack(components, comp->Clone());
	}
	*/
}

coEntity::~coEntity()
{
	for (coComponent* comp : coReversedLinkedComponents(this))
		delete comp;
}

void coEntity::SetTargetState(State newTargetState)
{
	targetState = newTargetState;
}

void coEntity::Write(coBinaryOutputStream& stream) const
{
	stream << uuid;
	const coUint32 nbComponents = GetNbComponents();
	stream << nbComponents;
	for (coComponent* comp : coLinkedComponents(this))
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

	coComponent* previous = this;
	for (coUint i = 0; i < nbComponents; ++i)
	{
		coUint32 nameHash;
		stream >> nameHash;
		const coType* type = coTypeFactory::instance->Get(nameHash);
		coASSERT(type);
		coComponent* comp = static_cast<coComponent*>(type->createFunc());
		stream >> *comp;
		comp->LinkTo(*previous);
		previous = comp;
	}
}
coResult coEntity::OnInit()
{
	for (coComponent* comp : coLinkedComponents(this))
	{
		coTRY(comp->Init(), nullptr);
	}
	return true;
}

void coEntity::OnRelease()
{
	for (coComponent* comp : coReversedLinkedComponents(this))
	{
		comp->Release();
	}
}

coResult coEntity::OnStart()
{
	for (coComponent* comp : coLinkedComponents(this))
	{
		coTRY(comp->Start(), nullptr);
	}
	return true;
}

void coEntity::OnStop()
{
	for (coComponent* comp : coReversedLinkedComponents(this))
	{
		comp->Stop();
	}
}

coResult coEntity::Write(coBinaryOutputStream& stream) const
{
	for (coComponent* comp : coLinkedComponents(this))
	{
		comp->Write(stream);
	}
	return true;
}

coResult coEntity::Read(coBinaryInputStream& stream)
{
	coASSERT(false);
	return true;
}

coEntity* coEntity::Clone() const
{
	//coEntity* clone = new coEntity(*this);
	coASSERT(false);
	return nullptr;
}

coUint coEntity::GetNbComponents() const
{
	coUint nb = 0;
	for (coComponent* comp : coLinkedComponents(this))
		++nb;
	return nb;
}
