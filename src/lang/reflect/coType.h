// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coSymbol.h"
#include "lang/reflect/coAttribute.h"
#include "lang/reflect/coFunction.h"
#include "container/array/coDynamicArray.h"

class coType : public coSymbol
{
public:
	typedef void* CreateFunc();
	coType();

	coUint32 size8;
	coType* super;
	CreateFunc* createFunc;
	coDynamicArray<coAttribute> attributes;
	coDynamicArray<coFunction> functions;
};
