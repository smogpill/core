// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityWorld.h"
#include <container/array/coDynamicArray_f.h>
#include "../processor/coEntityProcessor.h"
#include "../entity/coEntityBatch.h"
#include "../component/coComponent.h"
#include "../component/coComponentRegistry.h"
#include "container/coEntityContainer.h"
#include "processor/coEntityWorldProcessor.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coTypeRegistry.h"

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
            freeEntities[i] = newNbReservedEntities + i;

        nbReservedEntities = newNbReservedEntities;
    }

    const coUint32 entityIdx = coPopBack(freeEntities);
    const EntityInfo& info = entityInfos[entityIdx];
    return coEntityHandle(entityIdx, info.generation);
}

coEntityHandle coEntityWorld::CreateEntity(const coComponentMask& mask)
{
    const coEntityHandle entity = CreateEntity();
    EntityInfo& info = entityInfos[entity.index];
    info.type = GetOrCreateEntityType(mask);
    coEntityContainer* container = containers[info.type];
    info.indexInContainer = container->CreateEntity(entity);
    return entity;
}

void coEntityWorld::DestroyEntity(const coEntityHandle& entity)
{
    coASSERT(IsEntityAlive(entity));
    coASSERT(entity.index < nbReservedEntities);
    EntityInfo& info = entityInfos[entity.index];
    ++info.generation;
    if (info.generation != coUint32(-1))
    {
        coPushBack(freeEntities, entity.index);
    }
}

coBool coEntityWorld::IsEntityAlive(const coEntityHandle& entity) const
{
    coASSERT(entity.index < nbReservedEntities);
    return entityInfos[entity.index].generation == entity.generation;
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
        const auto& entityTypes = worldProcessor->entityTypes;
        batch.nbArrays = entityTypes.count;
        for (coUint arrayIdx = 0; arrayIdx < entityTypes.count; ++arrayIdx)
        {
            coEntityArray& array = batch.arrays[arrayIdx];
            const coEntityTypeID typeID = entityTypes[arrayIdx];
            coEntityContainer* container = containers[typeID];
            array.entities = container->entities;
            array.nbEntities = container->nbEntities;
        }
        worldProcessor->processor->OnUpdate(batch);
    }
}

coEntityTypeID coEntityWorld::GetOrCreateEntityType(const coComponentMask& mask)
{
    for (coUint32 containerIdx = 0; containerIdx < containers.count; ++containerIdx)
    {
        coEntityContainer* container = containers[containerIdx];
        if (container->componentMask == mask)
        {
            return containerIdx;
        }
    }

    // Create new container
    const coEntityTypeID typeID = containers.count;
    coEntityContainer* container = new coEntityContainer();
    container->componentMask = mask;
    coPushBack(containers, container);

    return typeID;
}
