// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../component/coComponentMask.h"
#include <debug/log/coAssert.h>
class coEntityHandle;
class coType;
class coComponent;
class coComponentTypeHandle;

class coEntityContainer
{
public:
	coEntityContainer(const coComponentMask& mask);
	~coEntityContainer();
	coUint32 CreateEntity(const coEntityHandle&);
	coUint16 GetIndexOfComponent(const coComponentTypeHandle&) const;
	void DestroyEntity(coUint32 index);
	void Reserve(coUint32 nb);
	coUint16 GetNbComponents() const { return nbComponents; }
	const coType* GetComponentType(coUint32 index) const { coASSERT(index < nbComponents); return componentTypes[index]; }
	coComponent* GetComponents(coUint32 index) const { coASSERT(index < nbComponents); return components[index]; }
	coComponent& GetComponent(coUint32 entityIdx, coUint32 componentTypeIdx) const;
	coComponent* FindComponent(coUint32 entityIdx, const coType& type) const;
	coUint FindComponentTypeIndex(const coType& type) const;

private:
	friend class coEntityWorld;

	coComponentMask componentMask;
	coEntityHandle* entities = nullptr;
	void* buffer = nullptr;
	coUint16 nbComponents = 0;
	coUint32 nbEntities = 0;
	coUint32 nbReservedEntities = 0;
	const coType** componentTypes = nullptr;
	coComponent** components = nullptr;
};
