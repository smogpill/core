// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <container/array/coDynamicArray.h>
#include "coEntityHandle.h"
class coEntityWorld;

class coEntityPackData
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityPackData);
public:
	const coArray<coEntityHandle>& GetHandles() const { return handles; }
	coBool IsValid() const { return handles.count == parentIndices.count; }

	coUint32 worldUid = 0;
	coEntityHandle rootEntity;
	coDynamicArray<coEntityHandle> handles;
	coDynamicArray<coUint32> parentIndices;
};

class coEntityDataContext
{
public:
	coEntityWorld* world = nullptr;
};