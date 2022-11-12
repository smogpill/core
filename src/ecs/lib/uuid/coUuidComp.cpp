// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coUuidComp.h"
#include "coUuidRegistry.h"
#include "../../entity/coEntity.h"
#include "../../component/coComponent_f.h"

coBEGIN_COMPONENT(coUuidComp);
coDEFINE_FIELD(uuid);
coDEFINE_COMPONENT_INIT();
coDEFINE_COMPONENT_SHUTDOWN();
coEND_COMPONENT();

void coUuidComp::Init(coEntity& entity)
{
	Base::Init(entity);
	if (uuid.IsValid())
	{
		coUuidRegistry* registry = coUuidRegistry::instance;
		coASSERT(registry);
		registry->SetEntity(uuid, entity.GetHandle());
	}
}

void coUuidComp::Shutdown(coEntity& entity)
{
	if (uuid.IsValid())
	{
		coUuidRegistry* registry = coUuidRegistry::instance;
		coASSERT(registry);
		registry->SetEntity(uuid, coEntityHandle::invalid);
	}
	Base::Shutdown(entity);
}
