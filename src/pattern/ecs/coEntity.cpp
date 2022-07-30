// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "coComponent.h"
#include "../uuid/coUuid_f.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/archive/coArchive.h>
#include <container/array/coFixedArray_f.h>
#include "component/coComponentIterator.h"

coDEFINE_CLASS(coEntity)
{
	coDEFINE_FIELD(uuid);
	coDEFINE_VIRTUAL_FIELD(components)
	{
		field->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
		{
			const coEntity* entity = static_cast<const coEntity*>(obj);
			const coUint32 nbComponents = entity->GetNbComponents();
			if (nbComponents == 0)
				return 0;
			const coUint32 index = archive.GetSize();
			archive.Write(nbComponents);
			auto& data = archive.GetData();
			const coUint32 offsetsIdx = archive.GetSize();
			archive.PushBytes(nbComponents * sizeof(coUint32));
			coUint32 itOffsetIdx = offsetsIdx;
			auto func = [&](coComponent& comp)
			{
				const coUint32 compIdx = archive.WriteObject(&comp, *comp.GetType());
				(coUint32&)(data[itOffsetIdx]) = compIdx - itOffsetIdx;
				itOffsetIdx += sizeof(coUint32);
				return true;
			};
			coVisitAll(entity->firstComponent, func);
			return index;
		};
		field->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
		{
		};
	}
}

coEntity::coEntity()
{
	static coUint64 uuidGenerator = 0;
	uuid.low = ++uuidGenerator;
}

coEntity::~coEntity()
{
	coCHECK(SetState(State::NONE), nullptr);
	auto func = [&](coComponent& comp)
	{
		delete& comp;
		return true;
	};
	coReverseVisitAll(firstComponent, func);
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

	auto func = [&](coComponent& comp)
	{
		const coType* type = comp.GetType();
		stream << type->nameHash;
		stream << comp;
		return true;
	};
	coVisitAll(firstComponent, func);
}

void coEntity::Read(coBinaryInputStream& stream)
{
	stream >> uuid;
	coUint32 nbComponents;
	stream >> nbComponents;

	coComponent* previous = nullptr;
	for (coUint i = 0; i < nbComponents; ++i)
	{
		coUint32 nameHash;
		stream >> nameHash;
		const coType* type = coTypeRegistry::instance->Get(nameHash);
		coASSERT(type);
		coComponent* comp = static_cast<coComponent*>(type->createFunc());
		stream >> *comp;
		if (previous)
		{
			previous->SetNextComponent(comp);
		}
		else
		{
			coASSERT(firstComponent == nullptr);
			firstComponent = comp;
		}
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
	coASSERT(GetState() == State::NONE);
	coEntity* entity = new coEntity();
	coASSERT(false);
	return entity;
}

coUint coEntity::GetNbComponents() const
{
	coUint nb = 0;
	auto func = [&](coComponent& comp)
	{
		++nb;
		return true;
	};
	coVisitAll(firstComponent, func);
	return nb;
}

coResult coEntity::Init()
{
	auto func = [&](coComponent& comp)
	{
		return comp.OnInit(*this);
	};
	return coVisitAll(firstComponent, func);
}

coResult coEntity::Start()
{
	auto func = [&](coComponent& comp)
	{
		return comp.OnStart(*this);
	};
	return coVisitAll(firstComponent, func);
}

void coEntity::Stop()
{
	auto func = [&](coComponent& comp)
	{
		comp.OnStop(*this);
		return true;
	};
	coReverseVisitAll(firstComponent, func);
}

void coEntity::Release()
{
	auto func = [&](coComponent& comp)
	{
		comp.OnRelease(*this);
		return true;
	};
	coReverseVisitAll(firstComponent, func);
}

void coEntity::Give(coComponent& component)
{
	if (firstComponent)
	{
		coComponent* last = firstComponent;
		while (last->nextComponent != firstComponent)
			last = last->nextComponent;
		last->nextComponent = &component;
	}
	else
	{
		firstComponent = &component;
	}
}
