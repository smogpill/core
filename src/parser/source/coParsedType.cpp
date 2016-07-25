// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/source/coParsedType.h"
#include "parser/source/coParsedField.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/reflect/coType.h"

coParsedType::coParsedType()
	: type(nullptr)
{

}

coParsedType::~coParsedType()
{
	delete type;
	for (auto* p : parsedFields)
		delete p;
}
