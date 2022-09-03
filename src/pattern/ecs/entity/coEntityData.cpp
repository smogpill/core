// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityData.h"
#include "lang/reflect/coTypeDecl_f.h"
#include "lang/reflect/coTypeRegistry.h"
#include "debug/log/coAssert.h"
#include <io/archive/coArchive.h>
#include "../world/coEntityWorld.h"
#include "../world/container/coEntityContainer.h"
#include "../component/coComponent.h"
#include "../component/ownership/coOwnership.h"
#include "coEntityType.h"
#include "coEntityTypeRegistry.h"

coUint32 coEntityData::Write(coArchive& archive, const coEntityHandle& entity)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	const coUint32 index = archive.GetSize();
	coEntityWorld::EntityInfo* info = world->GetEntityInfo(entity);
	const coEntityType* type = info->type;
	archive.Write(type->uid);
	const coEntityContainer* container = world->containers[info->patternID];
	const coUint16 nbComponents = container->GetNbComponents();
	archive.Write(nbComponents);
	const coUint32 inlineIdx = archive.GetSize();
	auto& archiveData = archive.GetData();
	archive.PushBytes(nbComponents * (sizeof(coUint32) * 2));
	for (coUint16 componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
	{
		const coType* componentType = container->GetComponentType(componentIdx);
		const coComponent& component = container->GetComponent(info->indexInContainer, componentIdx);
		const coUint32 dataIdx = inlineIdx + componentIdx * sizeof(coUint32) * 2;
		*reinterpret_cast<coUint32*>(&archiveData[dataIdx]) = componentType->uid;
		*reinterpret_cast<coUint32*>(&archiveData[dataIdx + sizeof(coUint32)]) = archive.WriteObject(&component, *componentType);
	}
	return index;
}

coEntityHandle coEntityData::Read(const coArchive& archive, coUint32 idx)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;

	const coUint32 entityTypeUID = archive.Get<coUint32>(idx);
	idx += sizeof(entityTypeUID);
	const coUint16 nbComponentsInData = archive.Get<coUint16>(idx);
	idx += sizeof(nbComponentsInData);
	const coEntityType* entityType = coEntityTypeRegistry::instance->Get(entityTypeUID);
	const coEntityHandle entity = world->CreateEntity(*entityType);
	const coEntityWorld::EntityInfo* info = world->GetEntityInfo(entity);
	const auto& archiveData = archive.GetData();
	coTypeRegistry* typeRegistry = coTypeRegistry::instance;
	coEntityContainer* container = world->containers[info->patternID];
	for (coUint16 componentDataIdx = 0; componentDataIdx < nbComponentsInData; ++componentDataIdx)
	{
		const coUint32* componentTypeUIDData = reinterpret_cast<const coUint32*>(&archiveData[idx + componentDataIdx * sizeof(coUint32) * 2]);
		const coUint32* componentIdxData = componentTypeUIDData + 1;

		if (*componentIdxData)
		{
			const coType* componentType = typeRegistry->Get(*componentTypeUIDData);
			if (componentType)
			{
				const coUint componentTypeIdx = container->FindComponentTypeIndex(*componentType);
				if (componentTypeIdx != coUint(-1))
				{
					coComponent& component = container->GetComponent(info->indexInContainer, componentTypeIdx);
					archive.ReadObject(*componentIdxData, &component, *componentType);
				}
			}
		}
	}

	return entity;
}

coUint32 coWriteEntityPackData(coArchive& archive, const void* obj)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	const coEntityPackData* data = static_cast<const coEntityPackData*>(obj);

	const coUint32 index = coEntityData::Write(archive, data->rootEntity);
	return index;
}

void coReadEntityPackData(const coArchive& archive, coUint32 idx, void* obj)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	coEntityPackData* data = static_cast<coEntityPackData*>(obj);
	data->rootEntity = coEntityData::Read(archive, idx);
}

coDEFINE_CLASS(coEntityPackData)
{
	type->writeArchiveFunc = &coWriteEntityPackData;
	type->readArchiveFunc = &coReadEntityPackData;
}
