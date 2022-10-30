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

void coEntity::SetState(State newState)
{
	SetTargetState(newState);
	while (state != newState)
		DoOneStateTransition();
}

coUint coEntity::GetNbComponents() const
{
	return entityType->GetComponentTypes().count;
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
			componentBuffer = new coComponent * [componentTypes.count];
			for (coUint compIdx = 0; compIdx < componentTypes.count; ++compIdx)
			{
				const coComponent* sourceComp = source.componentBuffer[compIdx];
				coASSERT(sourceComp);
				const coType* compType = componentTypes[compIdx];
				coComponent* comp = static_cast<coComponent*>(compType->copyCreateFunc(sourceComp));
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

void coEntity::SetChildrenState(State newState)
{
	coECS* ecs = coECS::instance;
	auto func = [&](coEntity& child)
	{
		child.SetState(newState);
		return true;
	};
	ecs->_VisitChildren(*this, func);
}

void coEntity::DoOneStateTransition()
{
	if (state == targetState)
		return;
	switch (state)
	{
	case State::NONE:
	{
		OnStateNoneToInitialized();
		break;
	}
	case State::INITIALIZED:
	{
		switch (targetState)
		{
		case State::NONE:
		{
			OnStateInitializedToNone();
			break;
		}
		case State::STARTED:
		{
			OnStateInitializedToStarted();
			break;
		}
		}
		break;
	}
	case State::STARTED:
	{
		OnStateStartedToInitialized();
		break;
	}
	default:
	{
		coASSERT(false);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
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
			const coComponentType* compTypeData = static_cast<const coComponentType*>(compType->customTypeData);
			if (compTypeData->stopFunc)
			{
				coComponent* component = componentBuffer[compIdx];
				coASSERT(component);
				compTypeData->stopFunc(*component);
			}
		}
	}
}

void coEntity::OnStateNoneToInitialized()
{
	InitComponents();
	state = State::INITIALIZED;
	SetChildrenState(State::INITIALIZED);
}

void coEntity::OnStateInitializedToNone()
{
	SetChildrenState(State::NONE);
	state = State::NONE;
	ShutdownComponents();
}

void coEntity::OnStateInitializedToStarted()
{
	StartComponents();
	state = State::STARTED;
	SetChildrenState(State::STARTED);
}

void coEntity::OnStateStartedToInitialized()
{
	SetChildrenState(State::INITIALIZED);
	state = State::INITIALIZED;
	StopComponents();
}
