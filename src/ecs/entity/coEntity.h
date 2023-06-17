// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
#include "../component/coComponent.h"
#include "coEntityHandle.h"
#include "coEntityDecl.h"
class coEntityType;

class coEntity
{
public:
	coEntity();
	void SetTargetState(coEntityState state_);

	coUint GetNbComponents() const;
	void** GetComponentBuffer() const { return componentBuffer; }
	coArray<void*> GetComponents() const { return coArray<void*>(componentBuffer, GetNbComponents()); }
	coEntityState GetState() const { return state; }
	coEntityState GetTargetState() const { return targetState; }
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
	coEntityState state = coEntityState::NIL;
	coEntityState targetState = coEntityState::NIL;
	coBool updateStateRequested : 1;
	void** componentBuffer = nullptr;
	const coEntityType* entityType = nullptr;
private:
	friend class coECS;

	void SetState(coEntityState state);
	void SetChildrenTargetState(coEntityState state);
	void SetChildrenState(coEntityState state);
	void InitComponents();
	void ShutdownComponents();
	void StartComponents();
	void StopComponents();
	void UpdateState();
	void OnStateUpdate_NIL();
	void OnStateUpdate_CREATED();
	void OnStateUpdate_READY();
	void OnStateUpdate_STARTED();
	void RequestUpdateState();
};

template <class T>
T* coEntity::GetComponent() const
{
	return static_cast<T*>(GetComponent(*T::GetStaticType()));
}
