// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coImgui
{
public:
	~coImgui();
	coResult Init();
	void Begin();
	void End();
	void DrawDemo();

private:
	coBool demo = false;
};
