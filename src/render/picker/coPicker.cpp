// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coPicker.h"
#include "render/shader/coShader.h"
#include "render/shader/coShaderProgram.h"
#include "render/context/coRenderContext.h"
#include "render/view/coRenderView.h"
#include "render/buffer/coFrameBuffer.h"
#include <lang/result/coResult_f.h>
#include <math/vector/coVec2.h>
#include <math/scalar/coUint32_f.h>

coPicker::~coPicker()
{
	delete frameBuffer;
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
}

coResult coPicker::Init(coRenderContext& context_)
{
	context = &context_;
	vertexShader = new coShader();
	coTRY(vertexShader->Init(coShader::Type::VERTEX, "shaders/render/Picker"), nullptr);
	fragmentShader = new coShader();
	coTRY(fragmentShader->Init(coShader::Type::FRAGMENT, "shaders/render/Picker"), nullptr);
	shaderProgram = new coShaderProgram();
	coDynamicArray<coShader*> shaderList;
	coPushBack(shaderList, vertexShader);
	coPushBack(shaderList, fragmentShader);
	coTRY(shaderProgram->Init(shaderList), nullptr);
	idShaderLocation = shaderProgram->GetUniformLocation("id");
	frameBuffer = new coFrameBuffer();
	coTRY(frameBuffer->Init(context_.GetMainRenderView()->GetSize(), coFrameBuffer::R32UI_D24_S8), nullptr);
	return true;
}

void coPicker::Begin()
{
	coASSERT(!started);
	context->Clear();
	shaderProgram->Bind();
	frameBuffer->Bind(coFrameBuffer::READ_WRITE);
	frameBuffer->Clear();
	started = true;
	glPointSize(8.0f);
	BindID(coUint32x2(0));
}

void coPicker::End()
{
	coASSERT(started);
	started = false;
	frameBuffer->Unbind();
	shaderProgram->Unbind();
}

void coPicker::SetModelViewProj(const coMat4& mvp)
{
	const auto modelViewProjLocation = shaderProgram->GetUniformLocation("modelViewProj");
	shaderProgram->SetUniform(modelViewProjLocation, mvp);
}

void coPicker::BindID(const coUint32x2 id)
{
	coASSERT(started);
	coASSERT(shaderProgram);;
	shaderProgram->SetUniform(idShaderLocation, id);
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
	
	frameBuffer->Bind(coFrameBuffer::READ_WRITE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	
	coUint32 value;
	glReadPixels(pos.x, pos.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &value);
	frameBuffer->Unbind();
	return value;
}

coRenderView* coPicker::GetView() const
{
	return context ? context->GetMainRenderView() : nullptr;
}
