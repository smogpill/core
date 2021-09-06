// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/context/coRenderContext.h"
#include "render/picker/coPicker.h"
#include "render/view/coRenderView.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "platform/coOs.h"

coRenderContext::~coRenderContext()
{
	delete picker;
	delete mainRenderView;
	if (hglrc)
	{
		if (!wglDeleteContext(hglrc))
		{
			coDynamicString str;
			coDumpLastOsError(str);
			coERROR("wglDeleteContext: " << str);
		}
	}
	if (hdc)
	{
		ReleaseDC(hwnd, hdc);
	}
}

coResult coRenderContext::Init(HWND _hwnd)
{
	hwnd = _hwnd;
	hdc = GetDC(_hwnd);
	coTRY(hdc, "GetDC(): Failed to retrieve the device context handle from the window handle.");

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	const int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("SetPixelFormat: " << str);
		return false;
	}

	coTRY(hglrc == NULL, nullptr);
	hglrc = wglCreateContext(hdc);
	if (hglrc == NULL)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("wglCreateContext: " << str);
		return false;
	}

	coTRY(Bind(), nullptr);
	coTRY(InitOpengl(), nullptr);
	mainRenderView = new coRenderView();
	RECT rect;
	if (GetClientRect(_hwnd, &rect) == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("GetClientRect: " << str);
		return false;
	}
	mainRenderView->SetViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

	picker = new coPicker();
	coTRY(picker->Init(*this), nullptr);
	return true;
}

coResult coRenderContext::BeginRender()
{
	coTRY(Bind(), nullptr);
	Clear();
	return true;
}

void coRenderContext::EndRender()
{
	if (!SwapBuffers(hdc))
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("SwapBuffers: " << str);
	}
}

void coRenderContext::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

coResult coRenderContext::Bind()
{
#ifdef coMSWINDOWS
	if (!wglMakeCurrent(hdc, hglrc))
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("SwapBuffers: " << str);
	}
	return true;
#endif
}

void coRenderContext::OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
	case GL_DEBUG_SEVERITY_MEDIUM:
	{
		coERROR(message);
		break;
	}
	case GL_DEBUG_SEVERITY_LOW:
	{
		coWARN(message);
		break;
	}
	case GL_DEBUG_SEVERITY_NOTIFICATION:
	{
		//coINFO(message);
		break;
	}
	}
}

coResult coRenderContext::InitOpengl()
{
	static coBool done = false;
	if (!done)
	{
		switch (gl3wInit())
		{
		case GL3W_OK: done = true; break;
		case GL3W_ERROR_INIT: coERROR("gl3w: Failed to init."); return false;
		case GL3W_ERROR_LIBRARY_OPEN: coERROR("gl3w: Failed to open lib gl"); return false;
		case GL3W_ERROR_OPENGL_VERSION: coERROR("gl3w: Failed to retrieve OpenGL version."); return false;
		default: return false;
		}
	}
	glEnable(GL_DEPTH_TEST);

#ifdef coDEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(OnDebugMessage, nullptr);
#endif

	//glFrontFace(GL_CW);
	
	return true;
}
