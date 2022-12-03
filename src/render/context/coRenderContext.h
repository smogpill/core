// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coVec3;
class coRenderView;
class coPicker;

class coRenderContext
{
public:
	~coRenderContext();
#ifdef coMSWINDOWS
	coResult Init(HWND hwnd);
#endif

	coResult BeginRender();
	void EndRender();
	coRenderView* GetMainRenderView() const { return mainRenderView; }
	coPicker* GetPicker() const { return picker; }

	void Clear();

private:
	coResult Bind();
	static void OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	static coResult InitOpengl();

#ifdef coMSWINDOWS
	HWND hwnd = nullptr;
	HDC hdc = nullptr;
	HGLRC hglrc = nullptr;
#endif
	coRenderView* mainRenderView = nullptr;
	coPicker* picker = nullptr;
	//coUint maxNbTextureUnits = 0;
};
