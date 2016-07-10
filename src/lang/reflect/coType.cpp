// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/pch.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coAttribute.h"
#include "lang/reflect/coFunction.h"
#include "container/array/coDynamicArray_f.h"

coType::coType()
	: size8(0)
	, super(nullptr)
	, createFunc(nullptr)
{

}

coType::~coType()
{
	coDeleteElementsAndClear(attributes);
	coDeleteElementsAndClear(functions);
}
