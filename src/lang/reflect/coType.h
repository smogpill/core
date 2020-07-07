// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coSymbol.h"
#include "container/array/coDynamicArray.h"
class coField;
class coFunction;
class coTypeBuilder;
typedef void* (*coCreateFunc)();

class coType : public coSymbol
{
public:
	~coType();

	coUint32 size8 = 0;
	coUint32 alignment8 = 0;
	const coType* super = nullptr;
	coCreateFunc createFunc = nullptr;
	coDynamicArray<coField*> fields;
	coDynamicArray<coFunction*> functions;
	coTypeBuilder* builder = nullptr;
};
