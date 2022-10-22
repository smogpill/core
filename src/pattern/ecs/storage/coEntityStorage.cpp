// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityStorage.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <io/archive/coArchive.h>
#include "../coECS.h"
#include "../entity/coEntityType.h"

class coComponentsStorage
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coComponentsStorage);
};

coDEFINE_CLASS(coComponentsStorage)
{
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntityStorage* entityStorage = static_cast<const coEntityStorage*>(obj);
		coECS* ecs = coECS::instance;
		const coEntity& entity = ecs->_GetEntity(entityStorage->index);
		const coEntityType* entityType = entity.entityType;
		const auto& componentTypes = entityType->GetComponentTypes();
		const coUint32 nbComponents = componentTypes.count;
		if (nbComponents == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(entityType->type->uid);
		archive.Write(nbComponents);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbComponents * 2 * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;
		for (coUint32 compIdx = 0; compIdx < nbComponents; ++compIdx)
		{
			const coComponent* comp = entity.componentBuffer[compIdx];
			const coType* compType = componentTypes[compIdx];
			(coUint32&)(data[itOffsetIdx]) = compType->uid;
			itOffsetIdx += sizeof(coUint32);
			const coUint32 compDataIdx = archive.WriteObject(&comp, *compType);
			(coUint32&)(data[itOffsetIdx]) = compDataIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
		}
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntityStorage* entityStorage = static_cast<coEntityStorage*>(obj);
		const coUint32* data = reinterpret_cast<const coUint32*>(&archive.GetData()[idx]);
		const coUint32 nbComponents = data[0];
		coTypeRegistry* typeRegistry = coTypeRegistry::instance;
		const coType* type = typeRegistry->Get(entityStorage->typeUID);
		const coEntityType* entityType = static_cast<const coEntityType*>(type->customTypeData);
		coASSERT(type);
		coECS* ecs = coECS::instance;
		const coEntityHandle entityHandle = ecs->CreateEntity(*type);
		coEntity& entity = ecs->_GetEntity(entityHandle.index);
		entityStorage->index = entityHandle.index;
		const coUint32* componentsBuffer = &data[1];
		entityType->GetComponentTypes();
		const coArray<coComponent*>& components = entity.GetComponents();
		for (coUint32 compIdx = 0; compIdx < nbComponents; ++compIdx)
		{
			const coUint32 compTypeUID = componentsBuffer[compIdx * 2 + 0];
			const coInt compIndex = entityType->GetComponentIndexByTypeUID(compTypeUID);
			if (compIndex >= 0)
			{
				coComponent* component = components[compIndex];
				coASSERT(component);
				const coUint32 compDataIdx = componentsBuffer[compIdx * 2 + 1];
				archive.ReadObject(compDataIdx, *component);
			}
		}
	};
}

coDEFINE_CLASS(coEntityStorage)
{
	coDEFINE_FIELD(parent);
	coDEFINE_FIELD(typeUID);
	coDEFINE_VIRTUAL_FIELD(components, coComponentsStorage);
}
