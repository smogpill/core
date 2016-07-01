// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"
#include "pattern/singleton/coSingleton.h"

class _coTestInfo;

class coTestRegistry
{
	coDECLARE_SINGLETON(coTestRegistry);
public:
	void Add(const _coTestInfo& _info);
	void RunAllTests();
	static void CreateInstanceIfMissing();
private:
	coDynamicArray<const _coTestInfo*> testInfos;
};
