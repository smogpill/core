// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

struct ImGuiContext;

class coImgui
{
public:
	~coImgui();
	coResult Init(HWND hwnd);
	void Begin();
	void End();
	void DrawDemo();
	void SetHWND(HWND hwnd_);
	LRESULT _ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	coBool UpdateMouseCursor();

	HWND hwnd = NULL;
	coBool demo = false;
	ImGuiContext* context = nullptr;
	ImGuiMouseCursor lastMouseCursor = ImGuiMouseCursor_COUNT;
};
