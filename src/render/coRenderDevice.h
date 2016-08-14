// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderDevice : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum DeviceType // Ordered from less powerful to more powerful
	{
		none,
		cpu,
		integratedGpu,
		discreteGpu,
	};
	class InitConfig : public Super::InitConfig
	{
	public:
	};

	//coRenderDevice();
	virtual ~coRenderDevice() {}

	virtual DeviceType GetDeviceType() const = 0;

protected:
	//virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
};
