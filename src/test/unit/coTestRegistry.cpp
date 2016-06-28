// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "test/pch.h"
#include "test/unit/coTestRegistry.h"
#include "test/unit/coTest.h"
#include "test/unit/internal/coTestInfo.h"
#include "container/array/coDynamicArray_f.h"

coTestRegistry* coTestRegistry::instance = nullptr;

void coTestRegistry::add(const _coTestInfo& _info)
{
	coPushBack(testInfos, &_info);
}

void coTestRegistry::runAllTests()
{
	for (const _coTestInfo* info : testInfos)
	{
		coASSERT(info);
		coTest* test = info->factory.createTest();
		coASSERT(test);
		test->executeBody();
		delete test;
	}
}

void coTestRegistry::createInstanceIfMissing()
{
	if (instance == nullptr)
	{
		instance = new coTestRegistry();
	}
}
