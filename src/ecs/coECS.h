// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <pattern/singleton/coSingleton.h>
#include "entity/coEntityHandle.h"
#include "entity/coEntity.h"
class coType;
class coComponent;
class coArchive;
class coProcessor;
class coEntityPackStorage;

class coECS
{
	coDECLARE_SINGLETON(coECS);
public:
	~coECS();
	void Reserve(coUint32 nbEntities);
	coEntityHandle CreateEntity(const coType&);
	template <class T>
	coEntityHandle CreateEntity() { return CreateEntity(*T::GetStaticType()); }
	void DestroyEntity(const coEntityHandle&);
	coEntityHandle Clone(const coEntityHandle&);
	void SaveEntity(coArchive& archive, const coEntityHandle&) const;
	coEntityHandle LoadEntity(const coArchive& archive);
	void SetParent(const coEntityHandle& child, const coEntityHandle& parent);
	coEntityHandle GetFirstChild(const coEntityHandle&) const;
	coEntityHandle GetPreviousSibling(const coEntityHandle&) const;
	coEntityHandle GetNextSibling(const coEntityHandle&) const;
	void Init(const coEntityHandle& entity);
	void Shutdown(const coEntityHandle& entity);
	void SetStarted(const coEntityHandle& entity, coBool);
	void AddProcessor(coProcessor& processor);
	coUint32 GetNbEntities(const coEntityHandle& root) const;

	coBool IsAlive(const coEntityHandle&) const;
	coComponent* GetComponent(const coEntityHandle&, const coType& type) const;
	template <class T>
	T* GetComponent(const coEntityHandle& entity) const { return static_cast<T*>(GetComponent(entity, *T::GetStaticType())); }
	coEntity& _GetEntity(coUint32 index) { return entities[index]; }
	const coEntity& _GetEntity(coUint32 index) const { return entities[index]; }
	coEntityHandle _CreateEmptyEntity();
	template <class F>
	coBool _VisitChildren(const coEntity& entity, F func) const;
	template <class F>
	coBool _ReverseVisitChildren(const coEntity& entity, F func);
private:
	coEntity* GetEntity(const coEntityHandle& handle) const;
	void DestroyEntity(coUint32 index);
	coUint32 GetNbEntities(coUint32 entityIndex) const;
	void SaveEntity(coEntityPackStorage& packStorage, coUint32 entityIndex, coUint32 parentEntityStorageIndex) const;

	coDynamicArray<coEntity> entities;
	coDynamicArray<coUint32> freeEntities;
	coDynamicArray<coProcessor*> processors;
};

template <class F>
coBool coECS::_VisitChildren(const coEntity& entity, F func) const
{
	coUint32 childIdx = entity.firstChild;
	if (childIdx != coUint32(-1))
	{
		do
		{
			coEntity& child = const_cast<coEntity&>(entities[childIdx]);
			const coUint32 nextIdx = child.nextSibling;
			if (!func(child))
				return false;
			childIdx = nextIdx;
		} while (childIdx != entity.firstChild);
	}
	return true;
}

template <class F>
coBool coECS::_ReverseVisitChildren(const coEntity& entity, F func)
{
	if (entity.firstChild != coUint32(-1))
	{
		coEntity& firstChild = entities[entity.firstChild];
		coUint32 childIdx = firstChild.previousSibling;
		do
		{
			coEntity& child = entities[childIdx];
			const coUint32 previousIdx = child.previousSibling;
			if (!func(child))
				return false;
			childIdx = previousIdx;
		} while (childIdx != firstChild.previousSibling);
	}
	return true;
}
