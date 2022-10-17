// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityPackStorage.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <io/archive/coArchive.h>
#include "../coECS.h"

class coComponentsStorage
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coComponentsStorage);
};

coDEFINE_CLASS(coComponentsStorage)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntityPackStorage* entityPack = static_cast<const coEntityPackStorage*>(obj);
		const coUint32 nbComponents = entityPack->components.count;
		if (nbComponents == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbComponents);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbComponents * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;

		coECS* ecs = coECS::instance;
		for (const coEntityStorage& entityStorage : entityPack->entities)
		{
			const coEntity& entity = ecs->_GetEntity(entityStorage.index);
		}
		for (coUint32 compIdx = 0; compIdx < nbComponents; ++compIdx)
		{
			const coComponent* comp = entity->componentBuffer[compIdx];
			const coType* compType = componentTypes[compIdx];
			const coUint32 compDataIdx = archive.WriteObject(&comp, *compType);
			(coUint32&)(data[itOffsetIdx]) = compDataIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
		}
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntityPackStorage* entityPack = static_cast<coEntityPackStorage*>(obj);
	};
}

coDEFINE_CLASS(coEntityPackStorage)
{
	coDEFINE_FIELD(entities);
	coDEFINE_VIRTUAL_FIELD(components, coComponentsStorage);
}
