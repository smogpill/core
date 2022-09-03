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

coUint32 coWriteEntityHandle(coArchive& archive, const coEntityWorld& world, const coEntityHandle& handle)
{
	const coUint32 index = archive.GetSize();
	coEntityWorld::EntityInfo* info = world.GetEntityInfo(handle);
	const coEntityType* type = info->type;
	archive.Write(type->uid);
	const coEntityContainer* container = world.containers[info->patternID];
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

coEntityHandle coReadEntityHandle(const coArchive& archive, coEntityWorld& world, coUint32 idx)
{
	const coUint32 entityTypeUID = archive.Get<coUint32>(idx);
	idx += sizeof(entityTypeUID);
	const coUint16 nbComponentsInData = archive.Get<coUint16>(idx);
	idx += sizeof(nbComponentsInData);
	const coEntityType* entityType = coEntityTypeRegistry::instance->Get(entityTypeUID);
	const coEntityHandle entity = world.CreateEntity(*entityType);
	const coEntityWorld::EntityInfo* info = world.GetEntityInfo(entity);
	const auto& archiveData = archive.GetData();
	coTypeRegistry* typeRegistry = coTypeRegistry::instance;
	coEntityContainer* container = world.containers[info->patternID];
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

void coGatherEntitiesAndParentIndicesRec(coEntityWorld& world, const coEntityHandle& handle, coDynamicArray<coEntityHandle>& handles, coDynamicArray<coUint32>& parentIndices, coUint32 parentIndex = coUint32(-1))
{
	coASSERT(!coContains(handles, handle));
	const coUint32 index = handles.count;
	coPushBack(handles, handle);
	coPushBack(parentIndices, parentIndex);
	coOwnership* ownership = world.FindComponent<coOwnership>();
	if (ownership)
	{
		// Siblings
		if (ownership->nextSibling.IsValid())
		{
			coGatherEntitiesAndParentIndicesRec(world, ownership->nextSibling, handles, parentIndices, parentIndex);
		}

		// Children
		if (ownership->firstChild.IsValid())
		{
			coGatherEntitiesAndParentIndicesRec(world, ownership->firstChild, handles, parentIndices, index);
		}
	}
}

coUint32 coWriteEntityData(coArchive& archive, const coEntityHandle& entity)
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

coUint32 coWriteEntityPackData(coArchive& archive, const void* obj)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	const coEntityPackData* data = static_cast<const coEntityPackData*>(obj);
	coASSERT(data->IsValid());

	const coUint32 index = archive.GetSize();

	coDynamicArray<coEntityHandle> handles;
	coDynamicArray<coUint32> parentIndices;
	coGatherEntitiesAndParentIndicesRec(*world, data->rootEntity, handles, parentIndices);

	for (coUint32 entityIdx = 0; entityIdx < handles.count; ++entityIdx)
	{
		const coEntityHandle handle = handles[entityIdx];
		const coUint32 index = coWriteEntityData(archive, handle);
	}
	
	return index;
}

coEntityHandle coReadEntityData(const coArchive& archive, coUint32 idx)
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

void coReadEntityPackData(const coArchive& archive, coUint32 idx, void* obj)
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

	coEntityData* data = static_cast<coEntityData*>(obj);
	data->handle = entity;
}

coDEFINE_CLASS(coEntityData)
{
	type->writeArchiveFunc = &coWriteEntityPackData;
	type->readArchiveFunc = &coReadEntityPackData;
}
