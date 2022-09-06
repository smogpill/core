// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include "pattern/singleton/coSingleton.h"
#include "container/array/coDynamicArray.h"

class coType;

class coTypeRegistry
{
	coDECLARE_SINGLETON(coTypeRegistry);
public:
	void Add(coType& type);
	coType* Get(coUint32 uid) const;
	const coArray<coType*>& GetTypes() const { return types; }
private:
	coDynamicArray<coType*> types;
};
