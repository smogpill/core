// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityHandle.h"
#include "coEntitySystem.h"
#include "coEntity.h"
#include "../uuid/coUuid_f.h"

/*
const coEntityHandle coEntityHandle::empty;

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