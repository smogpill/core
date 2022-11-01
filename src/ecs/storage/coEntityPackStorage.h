// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coTypeDecl.h>
#include "coEntityStorage.h"
class coComponent;

class coEntityPackStorage
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityPackStorage);
public:
	coDynamicArray<coEntityStorage> entities;
};
