// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coPicker.h"
#include "render/shader/coShader.h"
#include "render/shader/coShaderProgram.h"
#include "render/context/coRenderContext.h"
#include "render/view/coRenderView.h"
#include <lang/result/coResult_f.h>
#include <math/vector/coVec2.h>
#include <math/scalar/coUint32_f.h>

coPicker::~coPicker()
{
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
	coDynamicArray<const coShader*> shaderList;
	coPushBack(shaderList, vertexShader);
	coPushBack(shaderList, fragmentShader);
	coTRY(shaderProgram->Init(shaderList), nullptr);
	idShaderLocation = shaderProgram->GetUniformLocation("id");
	return true;
}

void coPicker::Begin()
{
	coASSERT(!started);
	context->Clear();
	shaderProgram->Bind();
	started = true;
	glPointSize(8.0f);
	BindID(coUint32x2(0));
}

void coPicker::End()
{
	coASSERT(started);
	started = false;
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
	const coUint32x4 value = PickValue(pos, GL_UNSIGNED_BYTE);
	return coColor(coUint8(value.x), coUint8(value.y), coUint8(value.z), coUint8(value.w));
}

coUint32x4 coPicker::PickID(const coVec2& pos) const
{
	return PickID(Convert(pos));
}

coUint32x4 coPicker::PickID(const coUint32x2& pos) const
{
	return PickValue(pos, GL_UNSIGNED_INT);
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

coUint32x4 coPicker::PickValue(const coUint32x2& pos, GLenum type) const
{
	const coRenderView* view = GetView();
	if (view == nullptr)
		return coUint32x4(0);

	const coUint width = view->GetWidth();
	const coUint height = view->GetHeight();
	if (width == 0 || height == 0)
		return coUint32x4(0);
	if (pos.x >= width || pos.y >= height)
		return coUint32x4(0);
	glFlush();
	glFinish();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	coUint32x4 data;
	switch (type)
	{
	case GL_UNSIGNED_BYTE:
	{
		coUint8 data8[4];
		glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, type, data8);
		data.x = data8[0];
		data.y = data8[1];
		data.z = data8[2];
		data.w = data8[3];
		break;
	}
	case GL_UNSIGNED_INT:
	{
		glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, type, &data.x);
		break;
	}
	default:
	{
		coASSERT(false);
		break;
	}
	}
	
	return data;
}

coRenderView* coPicker::GetView() const
{
	return context ? context->GetMainRenderView() : nullptr;
}
