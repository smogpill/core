// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "coComponent.h"
#include "lang/result/coResult_f.h"
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>

coEntity::~coEntity()
{
	for (coComponent* comp : components)
		delete comp;
}

void coEntity::AddAndGiveOwnership(coComponent& comp)
{
	coPushBack(components, &comp);
}

coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coEntity& entity)
{
	return stream;
}

coBinaryInputStream& operator>>(coBinaryInputStream& stream, coEntity& entity)
{
	return stream;
}
