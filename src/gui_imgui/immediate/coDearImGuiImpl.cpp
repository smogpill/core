// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "gui_imgui/pch.h"
#include "gui_imgui/immediate/coDearImGuiImpl.h"
#include "render/coRenderFactory.h"
#include "render/coShader.h"
#include "render/coRenderSampler.h"
#include "io/file/coFileAccess.h"
#include "io/file/coFile_f.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "memory/coMemory_f.h"
#include "gui/immediate/coImGui.h"

coDearImGuiImpl::coDearImGuiImpl()
	: vertexShader(nullptr)
	, fragmentShader(nullptr)
	, sampler(nullptr)
{

}

coDearImGuiImpl::~coDearImGuiImpl()
{
	delete sampler;
	delete vertexShader;
	delete fragmentShader;
}

coResult coDearImGuiImpl::InitShaders()
{
	coDynamicArray<coUint8> code;
	{
		coTRY(coReadFullFile(code, "shaders/coDebugGui.vert.spv"), nullptr);
		coShader* shader = coCreateShader();
		coDEFER() { delete shader; };
		coShader::InitConfig c;
		c.device = device;
		c.code = code;
		c.stage = coShader::vertex;
		coTRY(shader->Init(c), "Failed to init the vertex shader.");
		coSwap(vertexShader, shader);
	}

	{
		coTRY(coReadFullFile(code, "shaders/coDebugGui.frag.spv"), nullptr);
		coShader* shader = coCreateShader();
		coDEFER() { delete shader; };
		coShader::InitConfig c;
		c.device = device;
		c.code = code;
		c.stage = coShader::fragment;
		coTRY(shader->Init(c), "Failed to init the fragment shader.");
		coSwap(fragmentShader, shader);
	}

	return true;
}
