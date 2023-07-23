// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <pattern/singleton/coSingleton.h>
class coVec3;
class coRenderView;
class coRenderTexture;
class coRenderFrameBuffer;
class coRenderSampler;
class coPicker;
class coUint32x2;

class coRenderContext
{
	coDECLARE_SINGLETON(coRenderContext);
public:
	coRenderContext();
	~coRenderContext();
#ifdef coMSWINDOWS
	coResult Init(HWND hwnd);
#endif

	coResult BeginRender();
	void EndRender();
	coRenderView* GetMainRenderView() const { return mainRenderView; }
	coPicker* GetPicker() const { return picker; }

	void Clear();
	void BindTexture(coUint unit, const coRenderTexture&);
	void BindSampler(coUint unit, const coRenderSampler&);
	void UnbindTexture(coUint unit);
	void UnbindSampler(coUint unit);
	void UnbindAllTextures();
	void UnbindAllSamplers();
	void SetSize(const coUint32x2& size);
	static void SetGLDebugLabel(GLenum type, GLuint id, const coConstString& label);

	static const inline coUint8 maxNbTextureUnits = 32;

private:
	coResult Bind();
	static void OnDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	static coResult InitOpengl();
	void InitPixelFormatDefaults(PIXELFORMATDESCRIPTOR& descriptor) const;

	static constexpr coUint nbColorBits = 32;
	static constexpr coUint nbAlphaBits = 8;
	static constexpr coUint nbDepthBits = 24;
	static constexpr coUint nbStencilBits = 8;

#ifdef coMSWINDOWS
	HWND hwnd = nullptr;
	HDC hdc = nullptr;
	HGLRC hglrc = nullptr;
#endif

	coRenderView* mainRenderView = nullptr;
	coPicker* picker = nullptr;
	coUint32 boundTextureUnits = 0;
	coUint32 boundSamplerUnits = 0;
};
