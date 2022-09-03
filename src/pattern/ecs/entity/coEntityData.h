// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <container/array/coDynamicArray.h>
#include "coEntityHandle.h"
class coEntityWorld;
class coArchive;

class coEntityPackData
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityPackData);
public:
	coUint32 worldUid = 0;
	coEntityHandle rootEntity;
};

class coEntityData
{
public:
	static coUint32 Write(coArchive& archive, const coEntityHandle& entity);
	static coEntityHandle Read(const coArchive& archive, coUint32 idx);
};

class coEntityDataContext
{
public:
	coEntityWorld* world = nullptr;
};
