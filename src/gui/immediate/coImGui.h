// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "pattern/singleton/coSingleton.h"

class coImGuiImpl;
class coRenderCommandBuffer;
class coShader;
class coRenderDevice;

class coImGui : public coObject
{
	coDECLARE_SUPER(coObject);
	coDECLARE_SINGLETON(coImGui);
public:
	coImGui();
	coImGui(coImGuiImpl& _impl);
	virtual ~coImGui();

	void SetRenderDevice(coRenderDevice& _device);

	void BeginWindow(const coChar* _name);
	void EndWindow();
	void AddText(const coChar*);
	void AddSeparator();

	coResult BeginFrame();
	coResult EndFrame();
	coResult Render(coRenderCommandBuffer& _commandBuffer);

protected:
	virtual coResult OnInit() override;

	coImGuiImpl* impl;
};
