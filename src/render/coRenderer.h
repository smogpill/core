// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderDevice;
class coRenderContext;
class coRenderFramebuffer;
class coRenderCommandBuffer;

class coRenderer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderer();
	virtual ~coRenderer();

	class FillConfig
	{
	public:
		FillConfig();
		coRenderCommandBuffer* commandBuffer;
		coRenderFramebuffer* framebuffer;
	};

	coRenderContext* GetContext() const { return context; }

	virtual coResult FillCommandBuffer(const FillConfig& _config);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitDevices();

	coRenderContext* context;
	coDynamicArray<coRenderDevice*> devices;
};
