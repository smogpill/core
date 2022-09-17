// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coSymbol.h"
#include "container/array/coDynamicArray.h"
class coField;
class coFunction;
class coArchive;
using coCreateFunc = void* (*)();
using coMoveFunc = void (*)(const void*, void*);
using coConstructFunc = void (*)(void*);
using coDestructFunc = void (*)(void*);
using coWriteArchiveFunc = coUint32 (*)(coArchive&, const void*);
using coReadArchiveFunc = void (*)(const coArchive&, coUint32, void*);

class coCustomTypeData
{
public:
};

class coType : public coSymbol
{
public:
	coType();
	~coType();

	void Give(coField& field);
	void AddDependency(coType& type);
	template <class T>
	void AddDependency() { AddDependency(*T::GetStaticType()); }

	const coArray<coType*>& GetDependencies() const { return dependencies; }
	coUint GetNbSerializableFields() const;
	coBool IsCompatibleWith(const coType& type) const;
	template <class T>
	coBool IsCompatibleWith() const { return IsCompatibleWith(*T::GetStaticType()); }

	coUint32 size8 = 0;
	coUint32 alignment8 = 0;
	coUint32 indexInRegistry = coUint32(-1);
	coBool triviallyCopyable : 1;
	coCustomTypeData* customTypeData = nullptr;
	const coType* super = nullptr;
	const coType* subType = nullptr;
	coCreateFunc createFunc = nullptr;
	coMoveFunc moveFunc = nullptr;
	coConstructFunc constructFunc = nullptr;
	coDestructFunc destructFunc = nullptr;
	coWriteArchiveFunc writeArchiveFunc = nullptr;
	coReadArchiveFunc readArchiveFunc = nullptr;
	coDynamicArray<coField*> fields;
	coDynamicArray<coFunction*> functions;
	coDynamicArray<coType*> dependencies;
};
