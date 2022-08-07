// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponentMask.h"
#include "lang/reflect/coType.h"
#include "coComponent.h"
#include "coComponentRegistry.h"

void coComponentMask::Add(const coType& type)
{
	coASSERT(type.IsCompatibleWith<coComponent>());
	coComponentRegistry* registry = coComponentRegistry::instance;
	coASSERT(registry);
	Add(registry->GetHandle(type));
}

void coComponentMask::Add(const coComponentTypeHandle& handle)
{
	coASSERT(handle.IsValid());
	const coUint maskIndex = handle.index / 64;
	const coUint64 mask = 1ull << (handle.index % 64);
	masks[maskIndex] |= mask;
}

void coComponentMask::Remove(const coComponentTypeHandle& handle)
{
	coASSERT(handle.IsValid());
	const coUint maskIndex = handle.index / 64;
	const coUint64 mask = 1ull << (handle.index % 64);
	masks[maskIndex] &= ~mask;
}
