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
#include "../component/coComponentType.h"
#include "../entity/coEntityType.h"
#include "../coECS.h"

class coEntityType2
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityType2);
};

coDEFINE_CLASS(coEntityType2)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntity* entity = static_cast<const coEntity*>(obj);
		coASSERT(entity->entityType);
		const coUint32 index = archive.GetSize();
		archive.Write(entity->entityType->type->uid);
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 uid = archive.Get<coUint32>(idx);
		const coTypeRegistry* registry = coTypeRegistry::instance;
		coASSERT(registry);
		const coType* type = registry->Get(uid);
		entity->entityType = type ? static_cast<const coEntityType*>(type->customTypeData) : nullptr;
	};
}

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
		coASSERT(entity->entityType);
		const auto& componentTypes = entity->entityType->GetComponentTypes();
		const coUint32 nbComponents = componentTypes.count;
		if (nbComponents == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbComponents);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbComponents * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;
		for (coUint32 compIdx = 0; compIdx < nbComponents; ++compIdx)
		{
			const coComponent* comp = entity->componentBuffer[compIdx];
			const coType* compType = componentTypes[compIdx];
			const coUint32 compDataIdx = archive.WriteObject(&comp, *compType);
			(coUint32&)(data[itOffsetIdx]) = compDataIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
		}
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		if (entity->entityType == nullptr)
			return;
		entity->CreateComponents();
		const coUint32 nbComponents = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coUint32 compOffsetIdx = archive.Get<coUint32>(itIdx);
			if (compOffsetIdx)
			{
				const coType* type = archive.ReadObjectType(itIdx + compOffsetIdx);
				coASSERT(type);
				coComponent* comp = entity->GetComponent(*type);
				if (comp)
				{
					archive.ReadObject(itIdx + compOffsetIdx, comp, *type);
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
		coECS* ecs = coECS::instance;
		const coUint32 nbChildren = entity->GetNbChildren();
		if (nbChildren == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbChildren);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbChildren * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;

		coUint32 childIdx = entity->firstChild;
		while (childIdx != coUint32(-1))
		{
			const coEntity& child = ecs->_GetEntity(childIdx);
			const coUint32 childDataIdx = archive.WriteObject(&child, *child.entityType->type);
			(coUint32&)(data[itOffsetIdx]) = childIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
			childIdx = child.nextSibling;
		}
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coECS* ecs = coECS::instance;
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 nbChildren = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 childIdx = 0; childIdx < nbChildren; ++childIdx)
		{
			const coUint32 childOffsetIdx = archive.Get<coUint32>(itIdx);
			if (childOffsetIdx)
			{
				const coEntityHandle childEntityHandle = ecs->_CreateEmptyEntity();
				coEntity& child = ecs->_GetEntity(childEntityHandle.index);
				archive.ReadObject(itIdx + childOffsetIdx, child);
				ecs->SetParent(childEntityHandle, coEntityHandle(entity->index, entity->generation));
			}
			itIdx += sizeof(coUint32);
		}
	};
}

coDEFINE_CLASS(coEntity)
{
	coDEFINE_VIRTUAL_FIELD(type, coEntityType2);
	coDEFINE_VIRTUAL_FIELD(components, coEntityComponents);
	coDEFINE_VIRTUAL_FIELD(entities, coEntityChildren);
}

void coEntity::SetState(State newState)
{
	SetTargetState(newState);
	UpdateState();
}

coUint coEntity::GetNbComponents() const
{
	return entityType->GetComponentTypes().count;
}

coUint coEntity::GetNbChildren() const
{
	coECS* ecs = coECS::instance;
	coUint nbChildren = 0;
	coUint32 childIdx = firstChild;
	while (childIdx != coUint32(-1))
	{
		++nbChildren;
		const coEntity& child = ecs->_GetEntity(childIdx);
		childIdx = child.nextSibling;
	}
	return nbChildren;
}

void coEntity::UpdateState()
{
	if (state == targetState)
		return;
	switch (state)
	{
	case State::NONE:
	{
		switch (targetState)
		{
		case State::INITIALIZED:
		{
			Init();
			break;
		}
		case State::STARTED:
		{
			Init();
			Start();
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
			break;
		}
		case State::STARTED:
		{
			Start();
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
		{
			Stop();
			Shutdown();
			break;
		}
		case State::INITIALIZED:
		{
			Stop();
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
	state = targetState;
}

coComponent* coEntity::GetComponent(const coType& type) const
{
	const auto& componentTypes = entityType->GetComponentTypes();
	for (coUint componentIdx = 0; componentIdx < componentTypes.count; ++componentIdx)
	{
		if (componentTypes[componentIdx] == &type)
		{
			return componentBuffer[componentIdx];
		}
	}
	return nullptr;
}

void coEntity::CreateComponents()
{
	coASSERT(componentBuffer == nullptr);
	if (entityType)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		if (componentTypes.count)
		{
			componentBuffer = new coComponent*[componentTypes.count];
			for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
			{
				const coType* compType = componentTypes[compIdx];
				coComponent* comp = static_cast<coComponent*>(compType->createFunc());
				componentBuffer[compIdx] = comp;
			}
		}
	}
}

void coEntity::DestroyComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coInt compIdx = componentTypes.count - 1; compIdx >= 0; --compIdx)
		{
			delete componentBuffer[compIdx];
		}
		delete[] componentBuffer;
		componentBuffer = nullptr;
	}
}

void coEntity::Init()
{
	InitComponents();
}

void coEntity::Shutdown()
{
	ShutdownComponents();
}

void coEntity::Start()
{
	StartComponents();
}

void coEntity::Stop()
{
	StopComponents();
}

void coEntity::InitComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
		{
			const coType* compType = componentTypes[compIdx];
			const coComponentTypeData* compTypeData = static_cast<const coComponentTypeData*>(compType->customTypeData);
			if (compTypeData->initFunc)
			{
				coComponent* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->initFunc(*component);
			}
		}
	}
}

void coEntity::ShutdownComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coInt compIdx = componentTypes.count-1; compIdx >= 0; --compIdx)
		{
			const coType* compType = componentTypes[compIdx];
			const coComponentTypeData* compTypeData = static_cast<const coComponentTypeData*>(compType->customTypeData);
			if (compTypeData->shutdownFunc)
			{
				coComponent* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->shutdownFunc(*component);
			}
		}
	}
}

void coEntity::StartComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
		{
			const coType* compType = componentTypes[compIdx];
			const coComponentTypeData* compTypeData = static_cast<const coComponentTypeData*>(compType->customTypeData);
			if (compTypeData->startFunc)
			{
				coComponent* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->startFunc(*component);
			}
		}
	}
}

void coEntity::StopComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coInt compIdx = componentTypes.count - 1; compIdx >= 0; --compIdx)
		{
			const coType* compType = componentTypes[compIdx];
			const coComponentTypeData* compTypeData = static_cast<const coComponentTypeData*>(compType->customTypeData);
			if (compTypeData->stopFunc)
			{
				coComponent* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->stopFunc(*component);
			}
		}
	}
}
