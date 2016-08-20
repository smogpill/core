// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coInt32x2.h"

class coSwapChain;
class coRenderSemaphore;
class coSurface;
class coRenderContext;
class coRenderDevice;

class coRenderWindow : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coRenderContext* context;
		coInt32x2 size;
#ifdef coMSWINDOWS
		HWND hwnd;
#endif
	};
	coRenderWindow();
	virtual ~coRenderWindow();

	coResult Render();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult SelectDevice(const InitConfig& config);

	coSwapChain* swapChain;
	coSurface* surface;
	coRenderDevice* device;
	coRenderSemaphore* renderFinishedSemaphore;
};
