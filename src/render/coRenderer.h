// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderDevice;
class coRenderContext;

class coRenderer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coRenderer();
	virtual ~coRenderer();

	coResult Render();
	coRenderContext* GetContext() const { return context; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult InitDevices();

	coRenderContext* context;
	coDynamicArray<coRenderDevice*> devices;
};
