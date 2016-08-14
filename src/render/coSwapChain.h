// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coInt32x2.h"

class coSurface;
class coRenderDevice;

class coSwapChain : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coSurface* surface;
		coSwapChain* oldSwapChain;
		coInt32x2 size;
		coUint32 nbImages;
	};

	coSwapChain();
	virtual ~coSwapChain() {}

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coSurface* surface;
};
