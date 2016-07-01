// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class _coTestInfo;

class coTestRegistry
{
public:
	void Add(const _coTestInfo& _info);
	void RunAllTests();
	static void CreateInstanceIfMissing();

	static coTestRegistry* instance;
private:
	coDynamicArray<const _coTestInfo*> testInfos;
};
