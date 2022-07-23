// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "coTypeAutoRegistrator.h"
#include "coTypeRegistry.h"
#include <container/array/coDynamicArray_f.h>
#include <lang/result/coResult_f.h>

coDynamicArray<coTypeAutoRegistrator*>& coTypeAutoRegistrator::GetRegistrators()
{
	static coDynamicArray<coTypeAutoRegistrator*> registrators;
	return registrators;
}

void coTypeAutoRegistrator::AddRegistrator(coTypeAutoRegistrator& registrator)
{
	coPushBack(GetRegistrators(), &registrator);
}

coResult coTypeAutoRegistrator::RegisterAll()
{
	coTypeRegistry* typeRegistry = new coTypeRegistry();
	coTypeRegistry::instance = typeRegistry;
	for (coTypeAutoRegistrator* registrator : GetRegistrators())
	{
		coType* type = registrator->GetOrCreateType();
		coASSERT(type);
		coTRY(typeRegistry->Add(*type), nullptr);
	}
	return true;
}
