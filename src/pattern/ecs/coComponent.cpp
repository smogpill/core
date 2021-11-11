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

coResult coComponent::Init()
{
	return OnInit();
}

void coComponent::Release()
{
	OnRelease();
}

coResult coComponent::Start()
{
	return OnStart();
}

void coComponent::Write(coBinaryOutputStream& stream) const
{
}

void coComponent::Read(coBinaryInputStream& stream)
{
}

void coComponent::Stop()
{
	OnStop();
}

const coEntityHandle& coComponent::GetEntityHandle() const
{
	return entity ? entity->GetHandle() : coEntityHandle::empty;
}

coComponent* coComponent::Clone() const
{
	return nullptr;
}