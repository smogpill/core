// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coRenderContext
{
public:
	~coRenderContext();
#ifdef coMSWINDOWS
	coResult Init(HWND hwnd);
#endif

	coResult BeginRender();
	void EndRender();
	void Clear();

private:
	coResult Bind();

#ifdef coMSWINDOWS
	HWND hwnd = NULL;
	HDC hdc = NULL;
	HGLRC hglrc = NULL;
#endif
};
