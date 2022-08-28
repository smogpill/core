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
class coEntityType;
class coArchive;

class coEntityWorld
{
public:
	coEntityWorld();
	~coEntityWorld();

	// Setup
	void AddProcessor(coEntityProcessor&);

	// Update
	void Update();

	// Entity
	coEntityHandle CreateEntity();
	coEntityHandle CreateEntity(const coEntityType& type);
	template <class T>
	coEntityHandle CreateEntity() { return CreateEntity(*T::GetStaticType()); }
	void DestroyEntity(const coEntityHandle&);
	coBool IsEntityAlive(const coEntityHandle&) const;
	void Save(const coEntityHandle&, coArchive& out) const;
	coEntityHandle Load(const coArchive&);

private:
	friend class coEntityData;

	struct EntityInfo
	{
		coUint32 generation = 0;
		coEntityPatternID patternID = coEntityPatternID(-1);
		coUint32 indexInContainer = coUint32(-1);
		const coEntityType* type = nullptr;
	};
	coEntityPatternID GetOrCreateEntityPattern(const coComponentMask& mask);
	EntityInfo* GetEntityInfo(const coEntityHandle&) const;

	coDynamicArray<coEntityWorldProcessor*> processors;
	coDynamicArray<coEntityContainer*> containers;
	coDynamicArray<EntityInfo> entityInfos;
	coDynamicArray<coUint32> freeEntities;
	coUint32 nbReservedEntities = 0;
};
