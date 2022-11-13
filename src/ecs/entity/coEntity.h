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

class coEntity
{
public:
	enum class State : coUint8
	{
		NONE,
		INITIALIZED,
		STARTED,
		END
	};
	void SetState(State state);

	coUint GetNbComponents() const;
	void** GetComponentBuffer() const { return componentBuffer; }
	coArray<void*> GetComponents() const { return coArray<void*>(componentBuffer, GetNbComponents()); }
	State GetState() const { return state; }
	void* GetComponent(const coType& type) const;
	coEntityHandle GetHandle() const { return coEntityHandle(index, generation); }
	template <class T>
	T* GetComponent() const;
	void CreateComponents();
	void CreateComponents(const coEntity& source);
	void DestroyComponents();

	coUint32 index = coUint32(-1); // temporary
	coUint32 generation = 0;
	coUint32 firstChild = coUint32(-1);
	coUint32 previousSibling = coUint32(-1);
	coUint32 nextSibling = coUint32(-1);
	coUint32 parent = coUint32(-1);
	State state = State::NONE;
	State targetState = State::NONE;
	void** componentBuffer = nullptr;
	const coEntityType* entityType = nullptr;
private:
	friend class coECS;

	void SetChildrenState(State state);
	void SetTargetState(State state_) { targetState = state_; }
	void DoOneStateTransition();
	void InitComponents();
	void ShutdownComponents();
	void StartComponents();
	void StopComponents();
	void OnStateNoneToInitialized();
	void OnStateInitializedToStarted();
	void OnStateStartedToInitialized();
	void OnStateInitializedToNone();
};

template <class T>
T* coEntity::GetComponent() const
{
	return static_cast<T*>(GetComponent(*T::GetStaticType()));
}
