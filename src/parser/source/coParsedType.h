// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"
#include "container/string/coDynamicString.h"
#include "pattern/object/coObject.h"

class coType;
class coParsedField;

class coParsedType : public coObject
{
public:
	coParsedType();
	virtual ~coParsedType();

	coType* type;
	coDynamicArray<coParsedField*> parsedFields;
	coDynamicString superTypeName;
};
