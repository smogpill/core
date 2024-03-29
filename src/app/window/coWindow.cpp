// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/window/coWindow.h"
#include "lang/result/coResult_f.h"
#include "imgui/coImgui.h"
#include <math/vector/coUint32x2_f.h>
#include <render/context/coRenderContext.h>

coWindow::coWindow()
{
	OnImplConstruct();
}

coWindow::~coWindow()
{
	delete imgui;
	OnImplDestruct();
}

coResult coWindow::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	name = config.name;
	clientSize = config.clientSize;
	coTRY(OnImplInit(config), "Failed to init implementation.");
	return true;
}

coResult coWindow::CreateInstance()
{
	coTRY(OnImplInstanceCreate(), "The implementation failed to create an instance.");
	return true;
}

void coWindow::DestroyInstance()
{
	OnImplInstanceDestroy();
}

coResult coWindow::SetFullscreen(coBool b)
{
	if (fullscreen == b)
		return true;
	fullscreen = b;
	return true;
}

coResult coWindow::SetShowState(const ShowState& _state)
{
	if (showState == _state)
		return true;

	coTRY(OnImplApplyShowState(_state), nullptr);
	showState = _state;
	return true;
}

void coWindow::SetClientSize(const coUint32x2& newSize)
{
	if (clientSize != newSize)
	{
		clientSize = newSize;
		if (renderContext)
			renderContext->SetSize(clientSize);
	}
}