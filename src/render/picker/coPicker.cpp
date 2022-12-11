// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coPicker.h"
#include "render/shader/coShader.h"
#include "render/shader/coShaderProgram.h"
#include "render/context/coRenderContext.h"
#include "render/view/coRenderView.h"
#include "render/buffer/coFramebuffer.h"
#include <lang/result/coResult_f.h>
#include <math/vector/coVec2.h>
#include <math/scalar/coUint32_f.h>

coPicker::~coPicker()
{
	delete framebuffer;
	for (ModeInfo& modeInfo : modeInfos)
	{
		delete modeInfo.shaderProgram;
		delete modeInfo.fragmentShader;
		delete modeInfo.vertexShader;
	}
}

coResult coPicker::Init(coRenderContext& context_)
{
	context = &context_;

	// Vertex
	{
		ModeInfo& info = modeInfos[coUint(Mode::VERTEX)];
		info.vertexShader = new coShader();
		coTRY(info.vertexShader->Init(coShader::Type::VERTEX, "shaders/render/VertexPicker"), nullptr);
		info.fragmentShader = new coShader();
		coTRY(info.fragmentShader->Init(coShader::Type::FRAGMENT, "shaders/render/VertexPicker"), nullptr);
		info.shaderProgram = new coShaderProgram();
		coDynamicArray<coShader*> shaderList;
		coPushBack(shaderList, info.vertexShader);
		coPushBack(shaderList, info.fragmentShader);
		coTRY(info.shaderProgram->Init(shaderList), nullptr);
	}

	// Triangle
	{
		ModeInfo& info = modeInfos[coUint(Mode::TRIANGLE)];
		info.vertexShader = new coShader();
		coTRY(info.vertexShader->Init(coShader::Type::VERTEX, "shaders/render/TrianglePicker"), nullptr);
		info.fragmentShader = new coShader();
		coTRY(info.fragmentShader->Init(coShader::Type::FRAGMENT, "shaders/render/TrianglePicker"), nullptr);
		info.shaderProgram = new coShaderProgram();
		coDynamicArray<coShader*> shaderList;
		coPushBack(shaderList, info.vertexShader);
		coPushBack(shaderList, info.fragmentShader);
		coTRY(info.shaderProgram->Init(shaderList), nullptr);
	}

	// Mesh
	{
		ModeInfo& info = modeInfos[coUint(Mode::MESH)];
		info.vertexShader = new coShader();
		coTRY(info.vertexShader->Init(coShader::Type::VERTEX, "shaders/render/MeshPicker"), nullptr);
		info.fragmentShader = new coShader();
		coTRY(info.fragmentShader->Init(coShader::Type::FRAGMENT, "shaders/render/MeshPicker"), nullptr);
		info.shaderProgram = new coShaderProgram();
		coDynamicArray<coShader*> shaderList;
		coPushBack(shaderList, info.vertexShader);
		coPushBack(shaderList, info.fragmentShader);
		coTRY(info.shaderProgram->Init(shaderList), nullptr);
		info.idShaderLocation = info.shaderProgram->GetUniformLocation("id");
	}
	
	framebuffer = new coFramebuffer();
	coTRY(framebuffer->Init(context_.GetMainRenderView()->GetSize(), coFramebuffer::R32UI_D24_S8), nullptr);
	return true;
}

void coPicker::Begin(Mode mode)
{
	coASSERT(!started);
	currentMode = mode;
	context->Clear();
	ModeInfo& modeInfo = modeInfos[coUint(currentMode)];
	modeInfo.shaderProgram->Bind();
	framebuffer->Bind(coFramebuffer::READ_WRITE);
	framebuffer->Clear();
	started = true;
	glPointSize(8.0f);
}

void coPicker::End()
{
	coASSERT(started);
	started = false;
	ModeInfo& modeInfo = modeInfos[coUint(currentMode)];
	framebuffer->Unbind();
	modeInfo.shaderProgram->Unbind();
}

void coPicker::SetModelViewProj(const coMat4& mvp)
{
	ModeInfo& modeInfo = modeInfos[coUint(currentMode)];
	const auto modelViewProjLocation = modeInfo.shaderProgram->GetUniformLocation("modelViewProj");
	modeInfo.shaderProgram->SetUniform(modelViewProjLocation, mvp);
}

void coPicker::BindID(coUint32 id)
{
	coASSERT(started);
	ModeInfo& modeInfo = modeInfos[coUint(currentMode)];
	coASSERT(modeInfo.shaderProgram);
	modeInfo.shaderProgram->SetUniform(modeInfo.idShaderLocation, id);
}

void coPicker::BindID(const coUint32x2 id)
{
	coASSERT(started);
	ModeInfo& modeInfo = modeInfos[coUint(currentMode)];
	coASSERT(modeInfo.shaderProgram);
	modeInfo.shaderProgram->SetUniform(modeInfo.idShaderLocation, id);
}

coColor coPicker::PickColor(const coVec2& pos) const
{
	return PickColor(Convert(pos));
}

coColor coPicker::PickColor(const coUint32x2& pos) const
{
	return coColor(PickValue(pos));
}

coUint32 coPicker::PickID(const coVec2& pos) const
{
	return PickID(Convert(pos));
}

coUint32 coPicker::PickID(const coUint32x2& pos) const
{
	//coASSERT(started);
	return PickValue(pos);
}

coUint32x2 coPicker::Convert(const coVec2& pos) const
{
	const coRenderView* view = GetView();
	coASSERT(view);
	const coUint32 width = view->GetWidth();
	const coUint32 height = view->GetHeight();
	const coUint32 x = coClamp(coUint(((pos.x + 1.0f) * 0.5f) * width), 0, width);
	const coUint32 y = coClamp(coUint(((pos.y + 1.0f) * 0.5f) * height), 0, height);
	return coUint32x2(x, y);
}

coUint32 coPicker::PickValue(const coUint32x2& pos) const
{
	const coRenderView* view = GetView();
	if (view == nullptr)
		return 0;

	const coUint width = view->GetWidth();
	const coUint height = view->GetHeight();
	if (width == 0 || height == 0)
		return 0;
	if (pos.x >= width || pos.y >= height)
		return 0;
	glFlush();
	glFinish();
	
	framebuffer->Bind(coFramebuffer::READ_WRITE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	
	coUint32 value;
	glReadPixels(pos.x, pos.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &value);
	framebuffer->Unbind();
	return value;
}

coRenderView* coPicker::GetView() const
{
	return context ? context->GetMainRenderView() : nullptr;
}
