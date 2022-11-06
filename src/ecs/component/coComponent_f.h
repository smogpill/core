// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coComponent.h"
#include <lang/reflect/coTypeDecl_f.h>
#include "coComponentType.h"

#define coBEGIN_COMPONENT(_Class_) \
	coBEGIN_CLASS(_Class_)\
	coComponentType* component = new coComponentType(); \
	component->type = type; \
	type->customTypeData = component

#define coEND_COMPONENT() \
	coEND_CLASS()

#define coDEFINE_COMPONENT_INIT() component->initFunc = [](coEntity& entity, coComponent& comp) { static_cast<Self&>(comp).Init(entity); }
#define coDEFINE_COMPONENT_SHUTDOWN() component->shutdownFunc = [](coEntity& entity, coComponent& comp) { static_cast<Self&>(comp).Shutdown(entity); }
#define coDEFINE_COMPONENT_START() component->startFunc = [](coEntity& entity, coComponent& comp) { static_cast<Self&>(comp).Start(entity); }
#define coDEFINE_COMPONENT_STOP() component->stopFunc = [](coEntity& entity, coComponent& comp) { static_cast<Self&>(comp).Stop(entity); }

template <class T>
T* coComponent::GetComponent() const
{
	/*
	const coType* type = T::GetStaticType();
	for (coComponent* comp = nextComponent; comp != this; comp = comp->nextComponent)
	{
		if (comp->GetType() == type)
			return static_cast<T*>(comp);
	}
	*/
	return nullptr;
}
