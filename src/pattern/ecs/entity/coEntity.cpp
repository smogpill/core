// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/archive/coArchive.h>
#include <container/array/coFixedArray_f.h>
#include "../../uuid/coUuid_f.h"
#include "../component/coComponentIterator.h"
#include "../component/coComponent.h"

class coEntityComponents
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityComponents);
};

coDEFINE_CLASS(coEntityComponents)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
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
		coVisitAll(entity->GetFirstComponent(), func);
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 nbComponents = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coUint32 compOffsetIdx = archive.Get<coUint32>(itIdx);
			if (compOffsetIdx)
			{
				coComponent* component = archive.CreateObjects<coComponent>(itIdx + compOffsetIdx);
				if (component)
				{
					entity->Give(*component);
				}
			}
			itIdx += sizeof(coUint32);
		}
	};
}

class coEntityChildren
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityComponents);
};

coDEFINE_CLASS(coEntityChildren)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntity* entity = static_cast<const coEntity*>(obj);
		const coUint32 nbChildren = entity->GetNbChildren();
		if (nbChildren == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbChildren);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbChildren * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;
		const coType* entityType = coEntity::GetStaticType();
		auto func = [&](coEntity& child)
		{
			const coUint32 childIdx = archive.WriteObject(&child, *entityType);
			(coUint32&)(data[itOffsetIdx]) = childIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
			return true;
		};
		entity->VisitChildren(func);
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 nbChildren = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 childIdx = 0; childIdx < nbChildren; ++childIdx)
		{
			const coUint32 childOffsetIdx = archive.Get<coUint32>(itIdx);
			if (childOffsetIdx)
			{
				coEntity* child = archive.CreateObjects<coEntity>(itIdx + childOffsetIdx);
				if (child)
				{
					child->SetParent(entity);
				}
			}
			itIdx += sizeof(coUint32);
		}
	};
}

coDEFINE_CLASS(coEntity)
{
	coDEFINE_VIRTUAL_FIELD(components, coEntityComponents);
	coDEFINE_VIRTUAL_FIELD(entities, coEntityChildren);
}

coResult coEntity::SetState(State newState)
{
	while (state != newState)
	{
		coTRY(TransitToNextState(newState), nullptr);
	}
	return true;
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
			Shutdown();
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

void coEntity::Give(coComponent& component)
{
	if (firstComponent)
	{
		coComponent* last = firstComponent;
		while (last->nextComponent != firstComponent)
			last = last->nextComponent;
		last->nextComponent = &component;
		component.nextComponent = firstComponent;
	}
	else
	{
		firstComponent = &component;
	}
}

coComponent* coEntity::GetComponent(const coType& type) const
{
	coComponent* foundComp = nullptr;
	auto func = [&](coComponent& comp)
	{
		if (comp.GetType() == &type)
		{
			foundComp = &comp;
			return false;
		}
		return true;
	};
	coVisitAll(firstComponent, func);
	return foundComp;
}