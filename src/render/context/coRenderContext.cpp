// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/context/coRenderContext.h"
#include "render/picker/coPicker.h"
#include "render/view/coRenderView.h"
#include "render/texture/coRenderTexture.h"
#include "render/sampler/coRenderSampler.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"
#include <container/string/coDynamicString_f.h>
#include <math/scalar/coUint32_f.h>
#include <pattern/pointer/coUniquePtr.h>
#include "platform/coOs.h"

coDEFINE_SINGLETON(coRenderContext);

coRenderContext::coRenderContext()
{
	coRenderContext::instance = this;
}

coRenderContext::~coRenderContext()
{
	coRenderContext::instance = nullptr;
	delete picker;
	delete mainRenderView;
	if (hglrc)
	{
		if (!wglDeleteContext(hglrc))
		{
			coERROR("wglDeleteContext: " << coGetLastOSErrorMessage());
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

	PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
	pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = nbColorBits;
	pixelFormatDesc.cAlphaBits = nbAlphaBits;
	pixelFormatDesc.cDepthBits = nbDepthBits;
	pixelFormatDesc.cStencilBits = nbStencilBits;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

	// Dummy context to be able to make wgl calls
	{
		// Pixel format
		{
			const int pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDesc);
			if (SetPixelFormat(hdc, pixelFormat, &pixelFormatDesc) == FALSE)
			{
				coERROR("SetPixelFormat: " << coGetLastOSErrorMessage());
				return false;
			}
		}

		// Context
		{
			coTRY(hglrc == nullptr, nullptr);
			hglrc = wglCreateContext(hdc);
			if (hglrc == nullptr)
			{
				coERROR("wglCreateContext: " << coGetLastOSErrorMessage());
				return false;
			}
			wglMakeCurrent(hdc, hglrc);
		}
	}

	// Modern OpenGL context
	{
		// Pixel format
		{
			const int attribs[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, nbColorBits,
				WGL_ALPHA_BITS_ARB, nbAlphaBits,
				WGL_DEPTH_BITS_ARB, nbDepthBits,
				WGL_STENCIL_BITS_ARB, nbStencilBits,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				0 // End of attributes list
			};

			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");;
			coTRY(wglChoosePixelFormatARB, nullptr);
			int pixelFormat = 0;
			UINT nbFormats = 0;
			if (!wglChoosePixelFormatARB(hdc, attribs, nullptr, 1, &pixelFormat, &nbFormats))
			{
				coERROR("wglChoosePixelFormatARB: " << coGetLastOSErrorMessage());
				return false;
			}

			if (SetPixelFormat(hdc, pixelFormat, &pixelFormatDesc) == FALSE)
			{
				coERROR("SetPixelFormat: " << coGetLastOSErrorMessage());
				return false;
			}
		}

		// Context
		{
			const int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 5,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef coDEV
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#else
				WGL_CONTEXT_FLAGS_ARB, 0,
#endif
				0
			};

			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			coTRY(wglCreateContextAttribsARB, nullptr);
			HGLRC newContext = wglCreateContextAttribsARB(hdc, 0, attribs);
			if (newContext == NULL)
			{
				coERROR("wglCreateContextAttribs: " << coGetLastOSErrorMessage());
				return false;
			}

			wglMakeCurrent(hdc, newContext);
			wglDeleteContext(hglrc);
			hglrc = newContext;
		}
	}

	coTRY(Bind(), nullptr);
	coTRY(InitOpengl(), nullptr);
	mainRenderView = new coRenderView();
	RECT rect;
	if (GetClientRect(_hwnd, &rect) == FALSE)
	{
		coERROR("GetClientRect: " << coGetLastOSErrorMessage());
		return false;
	}
	mainRenderView->SetViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);

	coUniquePtr<coPicker> pickerPtr = new coPicker();
	coTRY(pickerPtr->Init(*this), nullptr);
	picker = pickerPtr.Release();
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
		coERROR("SwapBuffers: " << coGetLastOSErrorMessage());
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
		coERROR("SwapBuffers: " << coGetLastOSErrorMessage());
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
		coINFO(message);
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
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OnDebugMessage, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
#endif

	//int maxComputeTextureImageUnits = 0;
	//glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxComputeTextureImageUnits);
	//maxNbTextureUnits = maxComputeTextureImageUnits;
	
	return true;
}

void coRenderContext::InitPixelFormatDefaults(PIXELFORMATDESCRIPTOR& descriptor) const
{
	descriptor = PIXELFORMATDESCRIPTOR{};
	descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	descriptor.nVersion = 1;
	descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	descriptor.iPixelType = PFD_TYPE_RGBA;
	descriptor.cColorBits = nbColorBits;
	descriptor.cAlphaBits = nbAlphaBits;
	descriptor.cDepthBits = nbDepthBits;
	descriptor.cStencilBits = nbStencilBits;
	descriptor.iLayerType = PFD_MAIN_PLANE;
}

void coRenderContext::BindTexture(coUint unit, const coRenderTexture& texture)
{
	coASSERT(unit < maxNbTextureUnits);
	boundTextureUnits |= 1 << unit;
	glBindTextureUnit(unit, texture.GetGLID());
}

void coRenderContext::BindSampler(coUint unit, const coRenderSampler& sampler)
{
	coASSERT(unit < maxNbTextureUnits);
	boundSamplerUnits |= 1 << unit;
	glBindSampler(unit, sampler.GetGLID());
}

void coRenderContext::UnbindTexture(coUint unit)
{
	coASSERT(unit < maxNbTextureUnits);
	boundTextureUnits &= ~(1 << unit);
	glBindTextureUnit(unit, 0);
}

void coRenderContext::UnbindSampler(coUint unit)
{
	coASSERT(unit < maxNbTextureUnits);
	boundSamplerUnits &= ~(1 << unit);
	glBindSampler(unit, 0);
}

void coRenderContext::UnbindAllTextures()
{
	coUint32 units = boundTextureUnits;
	coUint8 unit = 0;
	while (units)
	{
		if (units & 1)
			UnbindTexture(unit);
		units >>= 1;
		++unit;
	}
	boundTextureUnits = 0;
}

void coRenderContext::UnbindAllSamplers()
{
	coUint32 units = boundSamplerUnits;
	coUint8 unit = 0;
	while (units)
	{
		if (units & 1)
			UnbindSampler(unit);
		units >>= 1;
		++unit;
	}
	boundSamplerUnits = 0;
}

void coRenderContext::SetGLDebugLabel(GLenum type, GLuint id, const coConstString& label)
{
#ifdef coDEV
	if (label.count)
	{
		coDynamicString s = label;
		coNullTerminate(s);
		glObjectLabel(type, id, -1, s.data);
	}
#endif
}

void coRenderContext::SetSize(const coUint32x2& size)
{
	if (mainRenderView)
	{
		const coUint32x2 pos = mainRenderView->GetPos();
		mainRenderView->SetViewport(pos.x, pos.y, size.x, size.y);
	}
	if (picker)
		picker->OnResize();
}