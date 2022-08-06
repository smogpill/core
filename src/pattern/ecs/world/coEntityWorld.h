// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "../entity/coEntityHandle.h"
#include "../coECSConfig.h"
class coEntityWorldProcessor;
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
	void Add(coEntityProcessor&);
	void Update();

private:
	coEntityTypeID GetOrCreateEntityType(const coComponentMask& mask);

	struct EntityInfo
	{
		coUint32 generation = 0;
		coEntityTypeID type = coEntityTypeID(-1);
		coUint32 indexInContainer = coUint32(-1);
	};

	coDynamicArray<coEntityWorldProcessor*> processors;
	coDynamicArray<coEntityContainer*> containers;
	coDynamicArray<EntityInfo> entityInfos; // TODO: Should be replaced by a direct array.
	coDynamicArray<coUint32> freeEntities;
	coUint32 nbReservedEntities = 0;
};
