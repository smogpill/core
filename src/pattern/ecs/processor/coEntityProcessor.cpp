// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityProcessor.h"
#include "lang/reflect/coType.h"
#include "../entity/coEntityBatch.h"
#include "../component/coComponent.h"
#include "../component/coComponentMask.h"
#include "../component/coComponentRegistry.h"

void coEntityProcessor::AddComponentType(const coType& type)
{
	coASSERT(type.IsCompatibleWith<coComponent>());
	coASSERT(!HasComponentType(type));
	coASSERT(nbComponentTypes < co_maxNbComponentsPerProcessor);
	const coComponentRegistry* registry = coComponentRegistry::instance;
	coASSERT(registry);
	componentTypeHandles[nbComponentTypes] = registry->GetHandle(type);
	++nbComponentTypes;
}

void coEntityProcessor::AddComponentTypeRW(const coType& type)
{
	static_assert(co_maxNbComponentsPerProcessor <= sizeof(componentTypesWriteAccess) * 8);
	componentTypesWriteAccess |= 1 << nbComponentTypes;
	AddComponentType(type);
}

coBool coEntityProcessor::HasComponentType(const coType& type) const
{
	coASSERT(type.IsCompatibleWith<coComponent>());
	const coComponentRegistry* registry = coComponentRegistry::instance;
	coASSERT(registry);
	const coComponentTypeHandle handle = registry->GetHandle(type);
	for (coUint32 typeIdx = 0; typeIdx < nbComponentTypes; ++typeIdx)
		if (componentTypeHandles[typeIdx] == handle)
			return true;
	return false;
}

coBool coEntityProcessor::IsCompatible(const coComponentMask& mask) const
{
	for (coUint32 typeIdx = 0; typeIdx < nbComponentTypes; ++typeIdx)
		if (!mask.HasType(componentTypeHandles[typeIdx]))
			return false;
	return true;
}

void coEntityProcessor::OnUpdate(const coEntityBatch& batch)
{
	for (coUint arrayIdx = 0; arrayIdx < batch.nbArrays; ++arrayIdx)
		OnUpdate(batch.arrays[arrayIdx]);
}
