// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "test/pch.h"
#include "test/unit/coTestRegistry.h"
#include "container/array/coDynamicArray_f.h"

coTestRegistry* coTestRegistry::instance = nullptr;

void coTestRegistry::add(const _coTestInfo& _info)
{
	coAdd(testInfos, &_info);
}

void coTestRegistry::createInstanceIfMissing()
{
	if (instance == nullptr)
	{
		instance = new coTestRegistry();
	}
}
