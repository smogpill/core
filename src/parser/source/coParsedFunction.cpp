// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/source/coParsedFunction.h"
#include "lang/reflect/coFunction.h"

coParsedFunction::coParsedFunction()
	: function(nullptr)
{

}

coParsedFunction::~coParsedFunction()
{
	delete function;
}
