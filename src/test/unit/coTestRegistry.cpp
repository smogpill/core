// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "test/pch.h"
#include "test/unit/coTestRegistry.h"
#include "test/unit/coTest.h"
#include "test/unit/internal/coTestInfo.h"
#include "container/array/coDynamicArray_f.h"

coDEFINE_SINGLETON(coTestRegistry);

void coTestRegistry::Add(const _coTestInfo& _info)
{
	coPushBack(testInfos, &_info);
}

void coTestRegistry::RunAllTests()
{
	for (const _coTestInfo* info : testInfos)
	{
		coASSERT(info);
		coTest* test = info->factory.CreateTest();
		coASSERT(test);
		test->ExecuteBody();
		delete test;
	}
}
