// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponent.h"
#include "coEntity.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeDecl_f.h"

coDEFINE_TYPE(coComponent)
{
	return true;
}

coComponent::coComponent()
{
	nextComponent = this;
}

coComponent::~coComponent()
{
	coASSERT(nextComponent == this);
}

void coComponent::Write(coBinaryOutputStream& stream) const
{
}

void coComponent::Read(coBinaryInputStream& stream)
{
}

void coComponent::LinkTo(coComponent& comp)
{
	coComponent* last = &comp;
	while (last->nextComponent != &comp)
		last = last->nextComponent;
	last->nextComponent = &comp;
}
