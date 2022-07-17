// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityHandle.h"
#include "coComponent.h"
#include <container/array/coDynamicArray.h>
#include <lang/reflect/coType.h>
#include <lang/result/coResult.h>
#include <pattern/uuid/coUuid.h>

class coBinaryOutputStream;
class coBinaryInputStream;

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
	coEntity();
	coEntity(const coEntity&) = delete;
	~coEntity();
	void Give(coComponent& component);
	void SetUuid(const coUuid& uuid_) { uuid = uuid_; }
	coResult SetState(State state);
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);
	coEntity* Clone() const;
	const coUuid& GetUuid() const { return uuid; }
	coUint GetNbComponents() const;
	State GetState() const { return state; }
	template <class T>
	T* GetComponent() const;

private:
	coResult Init();
	coResult Start();
	void Stop();
	void Release();
	coResult TransitToNextState(State targetState);
	coComponent* firstComponent = nullptr;
	coComponent* lastComponent = nullptr;
	State state = State::NONE;
	coUuid uuid;
};
