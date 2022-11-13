// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "coTypeAutoRegistrator.h"
#include "coTypeRegistry.h"
#include <container/array/coDynamicArray_f.h>
#include <lang/result/coResult_f.h>

coDynamicArray<coTypeAutoRegistrator*> co_registrators;

void coTypeAutoRegistrator::AddRegistrator(coTypeAutoRegistrator& registrator)
{
	coPushBack(co_registrators, &registrator);
}

coTypeAutoRegistrator* coTypeAutoRegistrator::GetRegistrator(coUint32 idx)
{
	return co_registrators[idx];
}

coUint coTypeAutoRegistrator::GetNbRegistrators()
{
	return co_registrators.count;
}