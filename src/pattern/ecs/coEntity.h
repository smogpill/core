// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityHandle.h"
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>
#include <lang/result/coResult.h>
#include <pattern/uuid/coUuid.h>

class coComponent;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntity
{
public:
	coEntity();
	~coEntity();
	void AddAndGiveOwnership(coComponent& comp);
	const coArray<coComponent*>& GetComponents() const { return components; }
	template <class T> T* GetComponent() const;
	const coEntityHandle& GetHandle() const { return handle; }
	void SetUuid(const coUuid& uuid_) { uuid = uuid_; }
	const coUuid& GetUuid() const { return uuid; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);
	void _OnSetHandle(const coEntityHandle& h) { handle = h; }
	coResult Init();
	void Release();
	coResult Start();
	void Stop();

private:
	coDynamicArray<coComponent*> components;
	coUuid uuid;
	coEntityHandle handle;
};

template <class T>
T* coEntity::GetComponent() const
{
	const coType* type = T::GetStaticType();
	const coUint32 hash = type->nameHash;
	for (coComponent* comp : components)
	{
		if (comp->GetType()->nameHash == hash)
			return static_cast<T*>(comp);
	}
	return nullptr;
}
