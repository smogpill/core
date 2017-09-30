// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coRenderMesh;

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

	virtual void PushExecuteCommands(const coArray<const coRenderCommandBuffer*>& _commandBuffers) = 0;
	virtual void PushDraw(const coRenderMesh& _mesh) = 0;
	virtual void PushSetScissor(coUint32 _x, coUint32 _y, coUint32 _width, coUint32 _height) = 0;

protected:
	coRenderCommandBuffer() {}
};
