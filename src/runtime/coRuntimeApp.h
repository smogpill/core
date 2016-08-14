// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "app/coApp.h"

class coRenderer;
class coWindow;
class coSurface;
class coSwapChain;
class coRenderDevice;

class coRuntimeApp : public coApp
{
	coDECLARE_SUPER(coApp);
public:
	coRuntimeApp();
	~coRuntimeApp();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;

private:
	coResult SelectRenderDevice();
	coResult Render();

	coWindow* window;
	coRenderer* renderer;
	coSurface* surface;
	coRenderDevice* renderDevice;
	coSwapChain* swapChain;
};
