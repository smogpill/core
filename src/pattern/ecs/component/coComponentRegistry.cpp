// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponentRegistry.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coTypeRegistry.h"
#include "container/array/coDynamicArray_f.h"
#include "coComponent.h"

coDEFINE_SINGLETON(coComponentRegistry);

coComponentRegistry::coComponentRegistry()
{
    coTypeRegistry::CreateInstanceIfMissing();
    const auto& allTypes = coTypeRegistry::instance->GetTypes();
    coResize(typeRegistryIndexToComponentTypeHandle, allTypes.count);
    for (const coType* type : allTypes)
    {
        if (type->IsCompatibleWith<coComponent>())
        {
            typeRegistryIndexToComponentTypeHandle[type->indexInRegistry] = coComponentTypeHandle(types.count);
            AddComponentType(*type);
        }
    }
}

coComponentTypeHandle coComponentRegistry::AddComponentType(const coType& type)
{
    coASSERT(type.IsCompatibleWith<coComponent>());
    coASSERT(!coContains(types, &type));
    coASSERT(type.triviallyCopyable);
    coComponentTypeHandle handle(coUint16(types.count));
    coPushBack(types, &type);
    return handle;
}

coComponentTypeHandle coComponentRegistry::GetHandle(const coType& type) const
{
    coASSERT(type.IsCompatibleWith<coComponent>());
    return typeRegistryIndexToComponentTypeHandle[type.indexInRegistry];
}
