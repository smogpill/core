// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
#include <pattern/uuid/coUuid.h>
#include "../component/coComponent.h"
#include "coEntityHandle.h"

// TODO:
// - Replace the entity by an ID (weak reference, 64 bits with 32 bits for generation, SOA)
// - Create entity contexts (network, visual, etc.). So that we can easily update it independently.

class coBinaryOutputStream;
class coBinaryInputStream;

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
	coEntity();
	coEntity(const coEntity&) = delete;
	~coEntity();
	void Give(coComponent& component);
	void SetParent(coEntity* entity);
	void Give(coEntity& entity);
	coResult SetState(State state);
	coEntity* Clone() const;
	coUint GetNbComponents() const;
	coUint GetNbChildren() const;
	coEntity* GetFirstChild() const { return firstChild; }
	coComponent* GetFirstComponent() const { return firstComponent; }
	State GetState() const { return state; }
	State GetParentStateWhenAttached() const { return parentStateWhenAttached; }
	coComponent* GetComponent(const coType& type) const;
	template <class T>
	T* GetComponent() const;
	template <class F>
	coBool VisitChildren(F) const;
	template <class F>
	coBool ReverseVisitChildren(F) const;

private:
	coResult Init();
	coResult Start();
	void Stop();
	void Shutdown();
	coResult TransitToNextState(State targetState);
	coComponent* firstComponent = nullptr;
	coEntity* firstChild = nullptr;
	coEntity* parent = nullptr;
	coEntity* previousSibling = nullptr;
	coEntity* nextSibling = nullptr;
	State state = State::NONE;
	State parentStateWhenAttached = State::NONE;
};

template <class T>
T* coEntity::GetComponent() const
{
	return static_cast<T*>(GetComponent(*T::GetStaticType()));
}

template <class F>
coBool coEntity::VisitChildren(F func) const
{
	if (firstChild)
	{
		coEntity* child = firstChild;
		do
		{
			coEntity* next = child->nextSibling;
			if (!func(*child))
				return false;
			child = next;
		} while (child != firstChild);
	}
	return true;
}

template <class F>
coBool coEntity::ReverseVisitChildren(F func) const
{
	if (firstChild)
	{
		coEntity* child = firstChild->previousSibling;
		do
		{
			coEntity* previous = child->previousSibling;
			if (!func(*child))
				return false;
			child = previous;
		} while (child != firstChild);
	}
	return true;
}
