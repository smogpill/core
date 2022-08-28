// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityWorld.h"
#include <container/array/coDynamicArray_f.h>
#include "../processor/coEntityProcessor.h"
#include "../entity/coEntityBatch.h"
#include "../entity/coEntityType.h"
#include "../entity/coEntityData.h"
#include "../component/coComponent.h"
#include "../component/coComponentRegistry.h"
#include "container/coEntityContainer.h"
#include "processor/coEntityWorldProcessor.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coTypeRegistry.h"
#include "io/archive/coArchive.h"

coEntityWorld::coEntityWorld()
{
    coComponentRegistry::CreateInstanceIfMissing();
}

coEntityWorld::~coEntityWorld()
{
    for (coEntityContainer* container : containers)
        delete container;
}

coEntityHandle coEntityWorld::CreateEntity()
{
    if (coUNLIKELY(freeEntities.count == 0))
    {
        const coUint32 nbNew = 256;
        const coUint32 newNbReservedEntities = nbReservedEntities + nbNew;
        coResize(freeEntities, nbNew);
        coResize(entityInfos, newNbReservedEntities);
        for (coUint32 i = 0; i < nbNew; ++i)
            freeEntities[i] = nbReservedEntities + nbNew - 1 - i;

        nbReservedEntities = newNbReservedEntities;
    }

    const coUint32 entityIdx = coPopBack(freeEntities);
    const EntityInfo& info = entityInfos[entityIdx];
    return coEntityHandle(entityIdx, info.generation);
}

coEntityHandle coEntityWorld::CreateEntity(const coEntityType& type)
{
    const coEntityHandle entity = CreateEntity();
    EntityInfo& info = entityInfos[entity.index];
    info.patternID = GetOrCreateEntityPattern(type.GetComponentMask());
    coEntityContainer* container = containers[info.patternID];
    info.indexInContainer = container->CreateEntity(entity);
    return entity;
}

void coEntityWorld::DestroyEntity(const coEntityHandle& entity)
{
    coASSERT(IsEntityAlive(entity));
    coASSERT(entity.index < nbReservedEntities);
    EntityInfo& info = entityInfos[entity.index];
    coEntityContainer* container = containers[info.patternID];
    container->DestroyEntity(info.indexInContainer);
    ++info.generation;
    if (info.generation != coUint32(-1))
    {
        coPushBack(freeEntities, entity.index);
    }
}

coBool coEntityWorld::IsEntityAlive(const coEntityHandle& entity) const
{
    if (entity.IsValid())
    {
        coASSERT(entity.index < nbReservedEntities);
        return entityInfos[entity.index].generation == entity.generation;
    }
    else
    {
        return false;
    }
}

void coEntityWorld::AddProcessor(coEntityProcessor& processor)
{
    coEntityWorldProcessor* entityWorldProcessor = new coEntityWorldProcessor();
    entityWorldProcessor->processor = &processor;
    coPushBack(processors, entityWorldProcessor);
}

void coEntityWorld::Update()
{
    coEntityBatch batch;
    for (const coEntityWorldProcessor* worldProcessor : processors)
    {
        coEntityProcessor* processor = worldProcessor->processor;
        const auto& entityTypeInfos = worldProcessor->entityTypeInfos;
        batch.nbArrays = entityTypeInfos.count;
        for (coUint infoIdx = 0; infoIdx < entityTypeInfos.count; ++infoIdx)
        {
            const coEntityWorldProcessor::EntityTypeInfo& info = entityTypeInfos[infoIdx];
            coEntityArray& array = batch.arrays[infoIdx];
            coEntityContainer* container = containers[info.entityTypeID];
            array.entities = container->entities;
            array.nbEntities = container->nbEntities;
            for (coUint componentIdx = 0; componentIdx < processor->nbComponentTypes; ++componentIdx)
            {
                const coUint componentIdxInEntityType = info.componentIndicesInEntityType[componentIdx];
                coASSERT(componentIdxInEntityType < container->nbComponents);
               array.components[componentIdx] = container->components[componentIdxInEntityType];
            }
        }
        processor->OnUpdate(batch);
    }
}

coEntityPatternID coEntityWorld::GetOrCreateEntityPattern(const coComponentMask& mask)
{
    for (coUint32 containerIdx = 0; containerIdx < containers.count; ++containerIdx)
    {
        coEntityContainer* container = containers[containerIdx];
        if (container->componentMask == mask)
        {
            return containerIdx;
        }
    }

    // Create new entity type
    const coEntityPatternID typeID = containers.count;
    coEntityContainer* container = new coEntityContainer(mask);
    container->componentMask = mask;
    coPushBack(containers, container);

    // Notify processors
    for (coEntityWorldProcessor* processor : processors)
    {
        processor->RegisterEntityType(typeID, *container, mask);
    }

    return typeID;
}

coEntityWorld::EntityInfo* coEntityWorld::GetEntityInfo(const coEntityHandle& entity) const
{
    if (entity.IsValid())
    {
        coASSERT(entity.index < nbReservedEntities);
        const EntityInfo* entityInfo = &entityInfos[entity.index];
        if (entityInfo->generation == entity.generation)
        {
            return const_cast<EntityInfo*>(entityInfo);
        }
    }
    return nullptr;
}

void coEntityWorld::Save(const coEntityHandle& entity, coArchive& out) const
{
    const EntityInfo* info = GetEntityInfo(entity);
    if (info)
    {
        coEntityData data;
        data.handle = entity;
        out.WriteRoot(data);
    }
}

coEntityHandle coEntityWorld::Load(const coArchive& in)
{
    const coUint32 rootIdx = in.GetRoot();
    coEntityData data;
    in.ReadObject(rootIdx, data);
    return data.handle;
}