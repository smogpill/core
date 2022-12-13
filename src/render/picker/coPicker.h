// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
#include <math/vector/coColor.h>
#include <math/vector/coUint32x2.h>
#include <math/vector/coUint32x4.h>
class coShader;
class coShaderProgram;
class coVec2;
class coMat4;
class coUint32x2;
class coRenderView;
class coRenderContext;
class coRenderFrameBuffer;

class coPicker
{
public:
	enum class Mode
	{
		VERTEX,
		TRIANGLE,
		MESH,

		END
	};
	~coPicker();
	coResult Init(coRenderContext& context);
	void Begin(Mode mode);
	void End();
	void SetModelViewProj(const coMat4& mvp);
	void BindID(coUint32 id);
	void BindID(const coUint32x2 id);
	coColor PickColor(const coVec2& pos) const;
	coColor PickColor(const coUint32x2& pos) const;
	coUint32 PickID(const coVec2& pos) const;
	coUint32 PickID(const coUint32x2& pos) const;

private:
	class ModeInfo
	{
	public:
		coShader* vertexShader = nullptr;
		coShader* fragmentShader = nullptr;
		coShaderProgram* shaderProgram = nullptr;
		coInt idShaderLocation = -1;
	};
	coUint32x2 Convert(const coVec2& pos) const;
	coUint32 PickValue(const coUint32x2& pos) const;
	coRenderView* GetView() const;
	coBool started = false;
	ModeInfo modeInfos[Mode::END];
	coRenderContext* context = nullptr;
	coRenderFrameBuffer* framebuffer = nullptr;
	Mode currentMode = Mode::MESH;
};