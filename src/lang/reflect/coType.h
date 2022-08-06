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

class coType : public coSymbol
{
public:
	coType();
	~coType();

	void Give(coField& field);
	coUint GetNbSerializableFields() const;
	coBool IsCompatibleWith(const coType& type) const;

	coUint32 size8 = 0;
	coUint32 alignment8 = 0;
	coBool triviallyCopyable : 1;
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
};
