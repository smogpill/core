// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "app/pch.h"
#include "app/coApp.h"

coResult coApp::ProcessEvents()
{
	MSG msg;
	if (::GetMessageW(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
	else
	{
		exitRequested = true;
		return msg.wParam == 0;
	}

	return true;
}
