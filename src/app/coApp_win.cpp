// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"

coResult coApp::ProcessEvents()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Translate the message and dispatch it to WindowProc()
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		exitRequested = true;
		return msg.wParam == 0;
	}

	return true;
}
