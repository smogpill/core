// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../singleton/coSingleton.h"
#include "entity/coEntityHandle.h"
class coEntityType;
class coComponent;

class coECS
{
	coDECLARE_SINGLETON(coECS);
public:
	coEntityHandle CreateEntity(const coEntityType&);
	template <class T>
	coEntityHandle CreateEntity() { return CreateEntity(*T::GetStaticType()); }
	coComponent* GetComponent(const coEntityHandle&);
private:
};
