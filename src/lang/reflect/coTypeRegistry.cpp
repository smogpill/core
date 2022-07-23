// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coTypeRegistry.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString_f.h"

coDEFINE_SINGLETON(coTypeRegistry);

coResult coTypeRegistry::Add(coType& _type)
{
	for (coType* e : types)
	{
		coTRY(e != &_type, "Type already in the type registry: " << _type.name);
		coTRY(e->nameHash != _type.nameHash, "There is already another type with the same hash as '" << _type.name << "': " << e->name);
	}
	coPushBack(types, &_type);
	return true;
}

coType* coTypeRegistry::Get(coUint32 _hash) const
{
	coTODO("Optimize by using a hash map");
	for (coType* e : types)
	{
		if (e->nameHash == _hash)
			return e;
	}

	return nullptr;
}
