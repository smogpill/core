// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderWorld.h"

coResult coRenderWorld::Add(coRenderEntity& _entity)
{
	coASSERT(!coContains(entities, &_entity));
	coPushBack(entities, &_entity);
	return true;
}

void coRenderWorld::Remove(coRenderEntity& _entity)
{
	coRemoveUnordered(entities, &_entity);
}
