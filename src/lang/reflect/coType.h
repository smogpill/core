// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coSymbol.h"
#include "container/array/coDynamicArray.h"

class coAttribute;
class coFunction;

class coType : public coSymbol
{
public:
	typedef void* CreateFunc();
	coType();
	virtual ~coType();

	coUint32 size8;
	coType* super;
	CreateFunc* createFunc;
	coDynamicArray<coAttribute*> attributes;
	coDynamicArray<coFunction*> functions;
};
