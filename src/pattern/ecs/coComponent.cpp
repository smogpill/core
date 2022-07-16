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

coResult coComponent::Init(coEntity& entity)
{
	coTRY(OnInit(entity), nullptr);
	if (nextComponent != &entity)
		coTRY(nextComponent->Init(entity), nullptr);
	return true;
}

coResult coComponent::Start(coEntity& entity)
{
	coTRY(OnStart(entity), nullptr);
	if (nextComponent != &entity)
		coTRY(nextComponent->Start(entity), nullptr);
	return true;
}

void coComponent::Stop(coEntity& entity)
{
	if (nextComponent != &entity)
		nextComponent->Stop(entity);
	OnStop(entity);
}

void coComponent::Release(coEntity& entity)
{
	if (nextComponent != &entity)
		nextComponent->Release(entity);
	OnRelease(entity);
}
