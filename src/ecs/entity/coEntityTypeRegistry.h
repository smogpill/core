// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <pattern/singleton/coSingleton.h>

class coEntityType;

class coEntityTypeRegistry
{
	coDECLARE_SINGLETON(coEntityTypeRegistry);
public:
	void Add(coEntityType& type);
	coEntityType* Get(coUint32 uid) const;
private:
	coDynamicArray<coEntityType*> entityTypes;
};
