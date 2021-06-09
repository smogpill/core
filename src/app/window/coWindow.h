// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coInt32x2.h"

class coRenderContext;
class coImgui;

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
		coConstString name;
		coInt32x2 clientSize = coInt32x2(1024, 768);
	};

	coWindow();
	virtual ~coWindow();

	coResult SetShowState(const ShowState& _state);
	ShowState GetShowState() const { return showState; }
	coResult SetForeground();
	coResult SetFocus();
	const coInt32x2& GetClientSize() const { return clientSize; }
	coRenderContext* GetRenderContext() const { return renderContext; };
	LRESULT _ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);
	coImgui* GetImgui() const { return imgui; }
	void Destroy();
#ifdef coMSWINDOWS
	void _SetHwnd(HWND h) { hwnd = h; }
#endif

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
	void ApplySize();

	ShowState showState = ShowState::hidden;
	coInt32x2 clientSize = coInt32x2(0);
	coDynamicString name;
	coImgui* imgui = nullptr;

#ifdef coMSWINDOWS
	HWND hwnd = NULL;
	HGLRC hglrc = NULL;
#endif
	coRenderContext* renderContext = nullptr;
};
