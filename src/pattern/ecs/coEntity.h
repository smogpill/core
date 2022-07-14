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

class coEntity : public coComponent
{
public:
	enum class State
	{
		NONE,
		INITIALIZED,
		STARTED
	};
	coEntity();
	coEntity(const coEntity&);
	~coEntity();
	coEntity* Clone() const;
	void SetUuid(const coUuid& uuid_) { uuid = uuid_; }
	void SetTargetState(State state);
	void Write(coBinaryOutputStream& stream) const override;
	void Read(coBinaryInputStream& stream) override;

	const coEntityHandle& GetHandle() const { return handle; }
	const coUuid& GetUuid() const { return uuid; }
	coUint GetNbComponents() const;
	State GetState() const { return state; }
	State GetTargetState() const { return targetState; }

	void _OnSetHandle(const coEntityHandle& h) { handle = h; }

protected:
	coResult OnInit() override;
	coResult OnStart() override;
	void OnStop() override;
	void OnRelease() override;

private:
	const coProcessorConfig* processorConfig = nullptr;
	coUuid uuid;
	coEntityHandle handle;
	State state = State::NONE;
	State targetState = State::NONE;
};
