// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/source/coParsedField.h"
#include "lang/reflect/coField.h"

coParsedField::coParsedField()
	: field(nullptr)
{

}

coParsedField::~coParsedField()
{
	delete field;
}
