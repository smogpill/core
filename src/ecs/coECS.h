// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <pattern/singleton/coSingleton.h>
#include <pattern/thread/coLock.h>
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
	coECS();
	~coECS();
	void Reserve(coUint32 nbEntities);
	coEntityHandle CreateEntity(const coType&);
	template <class T>
	coEntityHandle CreateEntity() { return CreateEntity(*T::GetStaticType()); }
	coEntityHandle CreateEmptyEntity();
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
	coLock& GetLock() const { return lock; }
	coBool IsAlive(const coEntityHandle&) const;
	void* GetComponent(const coEntityHandle&, const coType& type) const;
	template <class T>
	T* GetComponent(const coEntityHandle& entity) const { return static_cast<T*>(GetComponent(entity, *T::GetStaticType())); }
	coEntity& _GetEntity(coUint32 index) { return entityBlocks[index >> blockIndexShift]->entities[index & blockIndexMask]; }
	const coEntity& _GetEntity(coUint32 index) const { return entityBlocks[index >> blockIndexShift]->entities[index & blockIndexMask]; }
	template <class F>
	coBool _VisitChildren(const coEntity& entity, F func) const;
	template <class F>
	coBool _ReverseVisitChildren(const coEntity& entity, F func);
private:
	coEntity* GetEntity(const coEntityHandle& handle) const;
	void DestroyEntity(coUint32 index);
	coUint32 GetNbEntities(coUint32 entityIndex) const;
	void SaveEntity(coEntityPackStorage& packStorage, coUint32 entityIndex, coUint32 parentEntityStorageIndex) const;

	static constexpr coUint32 blockIndexShift = 16;
	static constexpr coUint32 nbEntitiesPerBlock = 1 << blockIndexShift;
	static constexpr coUint32 blockIndexMask = nbEntitiesPerBlock - 1;
	

	mutable coLock lock;
	struct EntityBlock
	{
		coEntity entities[nbEntitiesPerBlock];
	};

	coDynamicArray<EntityBlock*> entityBlocks;
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
			coEntity& child = const_cast<coEntity&>(_GetEntity(childIdx));
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
		coEntity& firstChild = _GetEntity(entity.firstChild);
		coUint32 childIdx = firstChild.previousSibling;
		do
		{
			coEntity& child = _GetEntity(childIdx);
			const coUint32 previousIdx = child.previousSibling;
			if (!func(child))
				return false;
			childIdx = previousIdx;
		} while (childIdx != firstChild.previousSibling);
	}
	return true;
}
