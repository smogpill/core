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

coEntity::~coEntity()
{
	coCHECK(SetState(State::NONE), nullptr);
	for (coComponent& comp : coReversedLinkedComponents(this))
		delete &comp;
}

coResult coEntity::SetState(State newState)
{
	while (state != newState)
	{
		coTRY(TransitToNextState(newState), nullptr);
	}
	return true;
}

void coEntity::Write(coBinaryOutputStream& stream) const
{
	stream << uuid;
	const coUint32 nbComponents = GetNbComponents();
	stream << nbComponents;
	for (coComponent& comp : coLinkedComponents(this))
	{
		const coType* type = comp.GetType();
		stream << type->nameHash;
		stream << comp;
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
		previous->SetNextComponent(comp);
		previous = comp;
	}
}

coResult coEntity::TransitToNextState(State targetState)
{
	// Should be more generic, like using a table of transitions

	switch (state)
	{
	case State::NONE:
	{
		switch (targetState)
		{
		case State::NONE: break;
		case State::INITIALIZED:
		case State::STARTED:
		{
			coTRY(Init(), nullptr);
			state = State::INITIALIZED;
			break;
		}
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	case State::INITIALIZED:
	{
		switch (targetState)
		{
		case State::NONE:
		{
			Release();
			state = State::NONE;
			break;
		}
		case State::INITIALIZED: break;
		case State::STARTED:
		{
			coTRY(Init(), nullptr);
			state = State::INITIALIZED;
			break;
		}
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	case State::STARTED:
	{
		switch (targetState)
		{
		case State::NONE:
		case State::INITIALIZED:
		{
			Stop();
			state = State::INITIALIZED;
			break;
		}
		case State::STARTED: break;
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	default:
	{
		coASSERT(false);
		break;
	}
	}
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
	for (coComponent& comp : coLinkedComponents(this))
		++nb;
	return nb;
}

coResult coEntity::Init()
{
	for (coComponent& comp : coLinkedComponents(this))
	{
		coTRY(comp.OnInit(*this), nullptr);
	}
	return true;
}

coResult coEntity::Start()
{
	for (coComponent& comp : coLinkedComponents(this))
	{
		coTRY(comp.OnStart(*this), nullptr);
	}
	return true;
}

void coEntity::Stop()
{
	for (coComponent& comp : coReversedLinkedComponents(this))
	{
		comp.OnStop(*this);
	}
}

void coEntity::Release()
{
	for (coComponent& comp : coReversedLinkedComponents(this))
	{
		comp.OnRelease(*this);
	}
}

void coEntity::Give(coComponent& component)
{
	coComponent* last = component;
	while (last->nextComponent != &component)
		last = last->nextComponent;
	last->nextComponent = &component;
}
