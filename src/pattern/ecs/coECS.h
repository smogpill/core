// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../singleton/coSingleton.h"
#include "entity/coEntityHandle.h"
#include "entity/coEntity.h"
class coEntityType;
class coComponent;
class coArchive;
class coProcessor;

class coECS
{
	coDECLARE_SINGLETON(coECS);
public:
	~coECS();
	void Reserve(coUint32 nbEntities);
	coEntityHandle CreateEntity(const coEntityType&);
	template <class T>
	coEntityHandle CreateEntity() { return CreateEntity(*T::GetStaticType()); }
	void DestroyEntity(const coEntityHandle&);
	coEntityHandle Clone(const coEntityHandle&);
	void SaveEntity(coArchive& archive, const coEntityHandle&) const;
	coEntityHandle LoadEntity(const coArchive& archive);
	void SetParent(const coEntityHandle& child, const coEntityHandle& parent);
	void SetStarted(const coEntityHandle& entity, coBool);
	void AddProcessor(coProcessor& processor);

	coBool IsAlive(const coEntityHandle&) const;
	coComponent* GetComponent(const coEntityHandle&, const coType& type) const;
	template <class T>
	T* GetComponent(const coEntityHandle& entity) const { return static_cast<T*>(GetComponent(entity, *T::GetStaticType())); }
private:
	coEntity* GetEntity(const coEntityHandle& handle) const;
	template <class F>
	coBool VisitChildren(const coEntity& entity, F func);
	template <class F>
	coBool ReverseVisitChildren(const coEntity& entity, F func);
	void DestroyEntity(coUint32 index);

	coDynamicArray<coEntity> entities;
	coDynamicArray<coUint32> freeEntities;
	coDynamicArray<coProcessor*> processors;
};

template <class F>
coBool coECS::VisitChildren(const coEntity& entity, F func)
{
	coUint32 childIdx = entity.firstChild;
	while (childIdx != coUint32(-1))
	{
		coEntity& child = entities[childIdx];
		const coUint32 nextIdx = child.nextSibling;
		if (!func(nextIdx))
			return false;
		childIdx = nextIdx;
	}
	return true;
}

template <class F>
coBool coECS::ReverseVisitChildren(const coEntity& entity, F func)
{
	if (entity.firstChild != coUint32(-1))
	{
		coEntity& firstChild = entities[entity.firstChild];
		if (firstChild.previousSibling == coUint32(-1))
		{
			return func(entity.firstChild);
		}
		else
		{
			coUint32 childIdx = firstChild.previousSibling;
			do
			{
				coEntity& child = entities[childIdx];
				const coUint32 previousIdx = child.previousSibling;
				if (!func(childIdx))
					return false;
				childIdx = previousIdx;
			} while (childIdx != coUint32(-1));
		}
	}
	return true;
}
