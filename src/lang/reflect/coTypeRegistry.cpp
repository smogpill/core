// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coTypeRegistry.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coTypeAutoRegistrator.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString_f.h"

coDEFINE_SINGLETON(coTypeRegistry);

void coTypeRegistry::Add(coType& type)
{
	for (coType* e : types)
	{
		coCHECK(e != &type, "Type already in the type registry: " << type.name);
		coCHECK(e->uid != type.uid, "There is already another type with the same uid as '" << type.name << "': " << e->name);
	}
	type.indexInRegistry = types.count;
	coPushBack(types, &type);
}

coType* coTypeRegistry::Get(coUint32 uid) const
{
	coTODO("Optimize by using a hash map");
	for (coType* e : types)
	{
		if (e->uid == uid)
			return e;
	}

	return nullptr;
}
