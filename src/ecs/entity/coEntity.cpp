// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coEntity.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/archive/coArchive.h>
#include <container/array/coFixedArray_f.h>
#include <pattern/uuid/coUuid_f.h>
#include "../component/coComponentIterator.h"
#include "../component/coComponent.h"
#include "../component/coComponentType.h"
#include "../entity/coEntityType.h"
#include "../coECS.h"

coEntity::coEntity()
	: updateStateRequested(false)
{
}

void coEntity::SetState(coEntityState newState)
{
	state = newState;
}

coUint coEntity::GetNbComponents() const
{
	return entityType->GetComponentTypes().count;
}

void* coEntity::GetComponent(const coType& type) const
{
	const auto& componentTypes = entityType->GetComponentTypes();
	for (coUint componentIdx = 0; componentIdx < componentTypes.count; ++componentIdx)
	{
		if (componentTypes[componentIdx]->IsCompatibleWith(type))
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
			componentBuffer = new void*[componentTypes.count];
			for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
			{
				const coType* compType = componentTypes[compIdx];
				//coComponent* comp = static_cast<coComponent*>(compType->createFunc());
				componentBuffer[compIdx] = compType->createFunc();
			}
		}
	}
}

void coEntity::CreateComponents(const coEntity& source)
{
	coASSERT(componentBuffer == nullptr);
	if (entityType)
	{
		coASSERT(source.entityType == entityType);
		const auto& componentTypes = entityType->GetComponentTypes();
		if (componentTypes.count)
		{
			coASSERT(source.componentBuffer);
			componentBuffer = new void * [componentTypes.count];
			for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
			{
				const void* sourceComp = source.componentBuffer[compIdx];
				coASSERT(sourceComp);
				const coType* compType = componentTypes[compIdx];
				//coComponent* comp = static_cast<coComponent*>(compType->copyCreateFunc(sourceComp));
				componentBuffer[compIdx] = compType->copyCreateFunc(sourceComp);
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

void coEntity::SetChildrenTargetState(coEntityState newState)
{
	coECS* ecs = coECS::instance;
	auto func = [&](coEntity& child)
	{
		child.SetTargetState(newState);
		return true;
	};
	ecs->_VisitChildren(*this, func);
}

void coEntity::SetChildrenState(coEntityState newState)
{
	coECS* ecs = coECS::instance;
	auto func = [&](coEntity& child)
	{
		child.SetState(newState);
		return true;
	};
	ecs->_VisitChildren(*this, func);
}

void coEntity::SetTargetState(coEntityState state_)
{
	if (targetState != state_)
	{
		targetState = state_;

		// Hack
		while (state != targetState)
		{
			UpdateState();
		}
	}
}

void coEntity::UpdateState()
{
	switch (state)
	{
	case coEntityState::NIL: OnStateUpdate_NIL(); break;
	case coEntityState::CREATED: OnStateUpdate_CREATED(); break;
	case coEntityState::READY: OnStateUpdate_READY(); break;
	case coEntityState::STARTED: OnStateUpdate_STARTED(); break;
	default:
	{
		break;
	}
	}
}

void coEntity::InitComponents()
{
	if (componentBuffer)
	{
		const auto& componentTypes = entityType->GetComponentTypes();
		for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
		{
			const coType* compType = componentTypes[compIdx];
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
			if (compTypeData->initFunc)
			{
				void* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->initFunc(*this, component);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
			if (compTypeData->shutdownFunc)
			{
				void* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->shutdownFunc(*this, component);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
			if (compTypeData->startFunc)
			{
				void* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->startFunc(*this, component);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
			if (compTypeData->stopFunc)
			{
				void* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->stopFunc(*this, component);
			}
		}
	}
}

void coEntity::OnStateUpdate_NIL()
{
	coASSERT(state == coEntityState::NIL);
	if (targetState != coEntityState::NIL)
	{
		SetState(coEntityState::CREATED);
		RequestUpdateState();
	}
}

void coEntity::OnStateUpdate_CREATED()
{
	coASSERT(state == coEntityState::CREATED);
	switch (targetState)
	{
	case coEntityState::NIL:
	{
		SetChildrenTargetState(coEntityState::NIL);
		SetState(coEntityState::NIL);
		break;
	}
	case coEntityState::CREATED: break;
	case coEntityState::READY:
	case coEntityState::STARTED:
	{
		//SetState(coEntityState::READYING);
		InitComponents();
		SetState(coEntityState::READY);
		SetChildrenTargetState(coEntityState::READY);
		break;
	}
	default:
	{
		coASSERT(false);
		break;
	}
	}

	if (targetState != state)
		RequestUpdateState();
}

void coEntity::OnStateUpdate_READY()
{
	coASSERT(state == coEntityState::READY);
	switch (targetState)
	{
	case coEntityState::NIL:
	case coEntityState::CREATED:
	{
		SetChildrenTargetState(coEntityState::CREATED);
		ShutdownComponents();
		SetState(coEntityState::CREATED);
		break;
	}
	case coEntityState::READY: break;
	case coEntityState::STARTED:
	{
		StartComponents();
		SetState(coEntityState::STARTED);
		SetChildrenTargetState(coEntityState::STARTED);
		break;
	}
	default:
	{
		coASSERT(false);
		break;
	}
	}
	if (targetState != state)
		RequestUpdateState();
}

void coEntity::OnStateUpdate_STARTED()
{
	coASSERT(state == coEntityState::STARTED);
	if (targetState != state)
	{
		SetChildrenTargetState(coEntityState::READY);
		StopComponents();
		SetState(coEntityState::READY);
	}
	if (targetState != state)
		RequestUpdateState();
}

void coEntity::RequestUpdateState()
{
	coECS::instance->RequestUpdateEntityState(index);
}
