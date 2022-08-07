// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
#include "pattern/singleton/coSingleton.h"
#include "coComponentTypeHandle.h"
class coType;

class coComponentRegistry
{
	coDECLARE_SINGLETON(coComponentRegistry);
public:
	coComponentRegistry();
	const coType* GetType(const coComponentTypeHandle handle) const { return types[handle.index]; }
	coComponentTypeHandle GetHandle(const coType& type) const;
	template <class T>
	coComponentTypeHandle GetHandle() const { return GetHandle(*T::GetStaticType()); }

private:
	coComponentTypeHandle AddComponentType(const coType& type);

	coDynamicArray<const coType*> types;
	coDynamicArray<coComponentTypeHandle> typeRegistryIndexToComponentTypeHandle;
};
