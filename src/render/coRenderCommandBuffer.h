// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coRenderCommandBuffer : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);

public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coBool primary;
	};
protected:
	coRenderCommandBuffer() {}
};
