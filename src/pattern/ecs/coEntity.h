// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>

class coComponent;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntity
{
public:
	~coEntity();
	void AddAndGiveOwnership(coComponent& comp);
	const coArray<coComponent*>& GetComponents() const { return components; }
	template <class T>
	T* GetComponent() const
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

	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

private:
	coDynamicArray<coComponent*> components;
};
