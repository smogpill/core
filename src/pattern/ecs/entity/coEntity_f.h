// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coEntity.h"
#include "../component/coComponentIterator.h"

template <class T>
T* coEntity::GetComponent() const
{
	const coType* type = T::GetStaticType();
	T* foundComp = nullptr;
	auto func = [&](coComponent& comp)
	{
		if (comp.GetType() == type)
		{
			foundComp = static_cast<T*>(&comp);
			return false;
		}
		return true;
	};
	coVisitAll(firstComponent, func);
	return foundComp;
}
