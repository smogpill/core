// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "math/vector/coInt32x2.h"

class coSwapChain;
class coRenderSemaphore;
class coSurface;
class coRenderer;
class coRenderDevice;
class coRenderContext;
class coRenderPipeline;
class coRenderPipelineLayout;
class coRenderPass;
class coShader;
class coRenderMaterial;
class coRenderWorld;

class coRenderWindow : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coRenderer* renderer;
		coInt32x2 size;
#ifdef coMSWINDOWS
		HWND hwnd;
#endif
	};
	coRenderWindow();
	virtual ~coRenderWindow();

	coResult Render(const coRenderWorld& _world);
	coRenderPass* GetPass() const;
	coRenderDevice* GetDevice() const { return device; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitPipelineLayout();
	coResult InitPipeline(coRenderPipeline& _out, const coRenderMaterial& _material);
	coResult InitSwapChain(const InitConfig& _config);
	coResult InitSemaphores();
	coResult InitSurface(const InitConfig& _config);
	coResult SelectDevice();
	coRenderContext* GetContext() const;

	coRenderer* renderer;
	coSwapChain* swapChain;
	coSurface* surface;
	coRenderDevice* device;
	coRenderSemaphore* renderFinishedSemaphore;
	coRenderPipelineLayout* pipelineLayout;
};