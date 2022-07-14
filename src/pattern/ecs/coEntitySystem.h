// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../singleton/coSingleton.h"
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
class coEntity;
class coEntityHandle;
class coUuid;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntitySystem
{
	coDECLARE_SINGLETON(coEntitySystem);
public:
	coEntitySystem();
	~coEntitySystem();
	const coEntityHandle& Give(coEntity& entity);
	coEntity* Get(const coEntityHandle& handle) const;
	const coEntityHandle& Get(const coUuid& uuid) const;
	template <class T>
	T* GetComponent(const coEntityHandle& handle) const;
	template <class T>
	T* GetComponent(const coUuid& uuid) const;
	coResult Init(const coEntityHandle& entity);
	coResult Init(const coArray<coEntityHandle>& entities);
	coResult Start(const coEntityHandle& entity);
	coResult Start(const coArray<coEntityHandle>& entities);
	coResult Save(const coEntityHandle& entity, coBinaryOutputStream& stream);
	coResult Load(const coEntityHandle& entity, coBinaryInputStream& stream);
	void Stop(const coEntityHandle& entity);
	void Stop(const coArray<coEntityHandle>& entities);
	void Release(const coEntityHandle& entity);
	void Release(const coArray<coEntityHandle>& entities);

private:
	coDynamicArray<coEntity*> entities;
};

template <class T>
T* coEntitySystem::GetComponent(const coEntityHandle& handle) const
{
	coEntity* entity = Get(handle);
	return entity ? entity->GetComponent<T>() : nullptr;
}

template <class T>
T* coEntitySystem::GetComponent(const coUuid& uuid) const
{
	return GetComponent<T>(Get(uuid));
}