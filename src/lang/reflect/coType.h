// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coSymbol.h"
#include "container/array/coDynamicArray.h"
class coField;
class coFunction;
class coArchive;
typedef void* (*coCreateFunc)();
typedef coUint32 (*coWriteArchiveFunc)(coArchive&, const void*);
typedef void (*coReadArchiveFunc)(const coArchive&, coUint32, void*);

class coType : public coSymbol
{
public:
	~coType();

	void Give(coField& field);
	coUint GetNbSerializableFields() const;
	coBool IsCompatibleWith(const coType& type) const;

	coUint32 size8 = 0;
	coUint32 alignment8 = 0;
	const coType* super = nullptr;
	const coType* subType = nullptr;
	coCreateFunc createFunc = nullptr;
	coWriteArchiveFunc writeArchiveFunc = nullptr;
	coReadArchiveFunc readArchiveFunc = nullptr;
	coDynamicArray<coField*> fields;
	coDynamicArray<coFunction*> functions;
};
