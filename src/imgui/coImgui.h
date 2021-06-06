// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

struct ImGuiContext;
class coWindow;

class coImgui
{
public:
	~coImgui();
	coResult Init();
	void Begin();
	void End();
	void DrawDemo();
	void SetWindow(coWindow* w) { window = w; }

private:
	coBool demo = false;
	ImGuiContext* context = nullptr;
	coWindow* window = nullptr;
};
