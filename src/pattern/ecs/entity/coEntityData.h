// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include "coEntityHandle.h"

class coEntityWorld;

class coEntityData
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityData);
public:
	coEntityHandle handle;
};

class coEntityDataContext
{
public:
	coEntityWorld* world = nullptr;
};