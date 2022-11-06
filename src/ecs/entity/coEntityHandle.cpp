// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coEntityHandle.h"
#include <pattern/uuid/coUuid_f.h>
#include "../coEntitySystem.h"
#include "coEntity.h"

const coEntityHandle coEntityHandle::invalid;

/*
void coEntityHandle::Write(coBinaryOutputStream& stream) const
{
	coASSERT(coEntitySystem::instance);
	const coEntity* entity = coEntitySystem::instance->Get(*this);
	stream << entity->GetUuid();
}

void coEntityHandle::Read(coBinaryInputStream& stream)
{
	coUuid uuid;
	stream >> uuid;
	coASSERT(coEntitySystem::instance);
	*this = coEntitySystem::instance->Get(uuid);
}

void coEntityHandle::DeepWrite(coBinaryOutputStream& stream) const
{
	const coEntity* entity = coEntitySystem::instance->Get(*this);
	stream << *entity;
}

void coEntityHandle::DeepRead(coBinaryInputStream& stream)
{
	coEntity* entity = new coEntity();
	stream >> *entity;
	*this = coEntitySystem::instance->Give(*entity);
}
*/