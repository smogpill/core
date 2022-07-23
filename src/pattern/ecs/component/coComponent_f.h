// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coComponent.h"
#include <lang/reflect/coTypeDecl_f.h>

#define coDEFINE_COMPONENT(_Class_) \
	coDEFINE_CLASS(_Class_)

template <class T>
T* coComponent::GetComponent() const
{
	const coType* type = T::GetStaticType();
	for (coComponent* comp = nextComponent; comp != this; comp = comp->nextComponent)
	{
		if (comp->GetType() == type)
			return static_cast<T*>(comp);
	}
	return nullptr;
}
