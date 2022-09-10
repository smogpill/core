// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coECS.h"
#include <debug/log/coAssert.h>

coDEFINE_SINGLETON(coECS);

coEntityHandle coECS::CreateEntity(const coEntityType& entityType)
{
	coASSERT(false);
	return coEntityHandle();
}

coComponent* coECS::GetComponent(const coEntityHandle& entityHandle)
{
	coASSERT(false);
	return nullptr;
}
