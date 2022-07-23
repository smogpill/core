// Copyright(c) 2016 Jounayd Id Salah
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
	coResult Add(coType& _type);
	coType* Get(coUint32 _hash) const;
private:
	coDynamicArray<coType*> types;
};
