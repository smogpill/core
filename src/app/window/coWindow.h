// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coInt32x2.h"

class coWindow : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum class ShowState
	{
		default,
		hidden,
		minimized,
		maximized
	};

	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coConstString name;
		coInt32x2 clientSize;
	};

	coWindow();
	virtual ~coWindow();

	coResult SetShowState(const ShowState& _state);
	ShowState GetShowState() const { return showState; }
	coResult SetForeground();
	coResult SetFocus();
	void* GetImpl() const { return impl; }
	const coInt32x2& GetClientSize() const { return clientSize; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	void OnImplConstruct();
	void OnImplDestruct();
	coResult OnImplInstanceCreate();
	void OnImplInstanceDestroy();
	coResult CreateInstance();
	void DestroyInstance();
	coResult OnImplInit(const InitConfig& _config);
	coResult OnImplApplyShowState(const ShowState& _state);

private:
	ShowState showState;
	coInt32x2 clientSize;
	coDynamicString name;
	void* impl;
};
