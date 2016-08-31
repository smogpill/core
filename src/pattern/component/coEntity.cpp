// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/component/coEntity.h"
#include "pattern/component/coComponent.h"
#include "lang/result/coResult_f.h"

coResult coEntity::Add(coComponent& _comp)
{
	coASSERT(!_comp.IsStarted());
	coASSERT(!_comp.GetEntity());
	coASSERT(coContains(components, &_comp));
	coPushBack(components, &_comp);
	_comp.entity = this;
	if (IsStarted())
	{
		coTRY(_comp.Start(), "Failed to start: "<<_comp);
	}
	return true;
}

void coEntity::Remove(coComponent& _comp)
{
	coASSERT(_comp.GetEntity() == this);
	coASSERT(coContains(components, &_comp));
	_comp.entity->Stop();
	_comp.entity = nullptr;
	coRemoveUnordered(components, &_comp);
}

coResult coEntity::OnStart()
{
	coTRY(Super::OnStart(), nullptr);
	for (coComponent* c : components)
	{
		const coBool b = c->Start();
		if (!b)
		{
			coERROR("Failed to start the component: " << *c);
			for (coComponent* c2 : components)
			{
				c2->Stop();
			}
			return false;
		}
	}
	return true;
}

void coEntity::OnStop()
{
	for (coComponent* c : components)
		c->Stop();
	Super::OnStop();
}