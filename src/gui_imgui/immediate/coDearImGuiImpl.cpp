// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "gui_imgui/pch.h"
#include "gui_imgui/immediate/coDearImGuiImpl.h"
#include "render/coRenderFactory.h"
#include "render/coShader.h"
#include "render/coRenderSampler.h"
#include "render/coRenderMaterial.h"
#include "render/coRenderImage.h"
#include "render/coRenderBuffer.h"
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
	, fontImage(nullptr)
	, fontUploadBuffer(nullptr)
{

}

coDearImGuiImpl::~coDearImGuiImpl()
{
	delete fontUploadBuffer;
	delete fontImage;
	delete material;
	delete sampler;
	delete vertexShader;
	delete fragmentShader;
}

coResult coDearImGuiImpl::InitMaterial()
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
		coTRY(shader->Init(c), "Failed to init the ImGui vertex shader.");
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
		coTRY(shader->Init(c), "Failed to init the ImGui fragment shader.");
		coSwap(fragmentShader, shader);
	}

	{
		coRenderMaterial* m = new coRenderMaterial();
		coDEFER() { delete m; };
		coRenderMaterial::InitConfig c;
		c.vertexShader = vertexShader;
		c.fragmentShader = fragmentShader;
		c.debugName = "DearImGui material";
		m->vertexInput = coRenderVertexInput::POS2_UV2_COLOR1;
		coTRY(m->Init(c), "Failed to init the ImGui material.");
		coSwap(material, m);
	}

	{
		sampler = coCreateRenderSampler();
		coRenderSampler::InitConfig c;
		c.debugName = "ImGui ";
		c.device = device;
		coTRY(sampler->Init(c), "Failed to init the ImGui sampler.");
	}

	return true;
}

coResult coDearImGuiImpl::InitFont()
{
	ImGuiIO& io_di = ImGui::GetIO();
	coUchar* pixels;
	coInt width, height;
	io_di.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	const coUint size = width * height * 4;

	// Font image
	{
		coRenderImage* p = coCreateRenderImage();
		coDEFER() { delete p; };
		coRenderImage::InitConfig c;
		c.size.x = width;
		c.size.y = height;
		c.size.z = 1;
		c.type = coRenderImage::default_;
		c.debugName = "";
		c.device = device;
		coTRY(p->Init(c), "Failed to init the font render image for the ImGui.");
		coSwap(p, fontImage);
	}
	
	// Font image view
	{
		
	}

	// Font data
	{
		coRenderBuffer* p = coCreateRenderBuffer();
		coDEFER() { delete p; };
		coRenderBuffer::InitConfig c;
		c.size8 = size;
		c.type = coRenderBuffer::staging;
		c.device = device;
		c.debugName = "ImGuiFontUploadBuffer";
		coTRY(p->Init(c), "Failed to init the font upload buffer.");
		coSwap(p, fontUploadBuffer);

		coByte* data;
		coTRY(fontUploadBuffer->Map((void*&)data), nullptr);
		coMemCopy(data, pixels, size);
		coCHECK(fontUploadBuffer->FlushMapped(), nullptr);
		fontUploadBuffer->Unmap();
	}

	return true;
}
