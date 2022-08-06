// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../entity/coEntityHandle.h"
#include <container/array/coDynamicArray.h>
class coEntityProcessor;
class coEntityContainer;
class coComponentMask;

class coEntityWorld
{
public:
	~coEntityWorld();
	coEntityHandle CreateEntity();
	coEntityHandle CreateEntity(const coComponentMask& mask);
	void DestroyEntity(const coEntityHandle&);
	coBool IsEntityAlive(const coEntityHandle&) const;
	void Update();

private:
	coUint32 GetOrCreateContainer(const coComponentMask& mask);

	struct EntityInfo
	{
		coUint32 generation = 0;
		coUint32 containerIdx = coUint32(-1);
		coUint32 indexInContainer = coUint32(-1);
	};

	coDynamicArray<coEntityProcessor*> processors;
	coDynamicArray<coEntityContainer*> containers;
	coDynamicArray<EntityInfo> entityInfos; // Should be replaced by a direct array.
	coDynamicArray<coUint32> freeEntities;
	coUint32 nbReservedEntities = 0;
};
