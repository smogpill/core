// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coSymbol.h"
#include "container/array/coDynamicArray.h"

class coField;
class coFunction;

typedef void* (*coCreateFunc)();

class coType : public coSymbol
{
public:
	coType();
	~coType();

	coUint32 size8;
	coUint32 alignment8;
	const coType* super;
	coCreateFunc createFunc;
	coDynamicArray<coField*> fields;
	coDynamicArray<coFunction*> functions;
};
