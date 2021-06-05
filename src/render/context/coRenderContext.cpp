// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/context/coRenderContext.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include "platform/coOs.h"

coRenderContext::~coRenderContext()
{
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
