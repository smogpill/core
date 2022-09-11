// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
#include "../component/coComponent.h"
#include "coEntityHandle.h"
class coEntityType;

// TODO:
// - Replace the entity by an ID (weak reference, 64 bits with 32 bits for generation, SOA)
// - Create entity contexts (network, visual, etc.). So that we can easily update it independently.

class coEntity
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntity);
public:
	enum class State : coUint8
	{
		NONE,
		INITIALIZED,
		STARTED,
		END
	};
	coResult SetState(State state);
	coUint GetNbComponents() const;
	void Give(coComponent& component);
	coComponent* GetFirstComponent() const { return firstComponent; }
	State GetState() const { return state; }
	State GetParentStateWhenAttached() const { return parentStateWhenAttached; }
	coComponent* GetComponent(const coType& type) const;
	template <class T>
	T* GetComponent() const;

private:
	friend class coECS;

	coResult TransitToNextState(State targetState);
	coUint32 generation = 0;
	coUint32 firstChild = coUint32(-1);
	coUint32 previousSibling = coUint32(-1);
	coUint32 nextSibling = coUint32(-1);
	coUint32 parent = coUint32(-1);
	State state = State::NONE;
	State parentStateWhenAttached = State::NONE;
	coComponent* firstComponent = nullptr;
	const coEntityType* entityType = nullptr;
};

template <class T>
T* coEntity::GetComponent() const
{
	return static_cast<T*>(GetComponent(*T::GetStaticType()));
}
