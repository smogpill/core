// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coLogicalDevice;

class coRenderBuffer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	enum Usage
	{
		vertex = 1<<0,
		index = 1<<1,
		uniform = 1<<2,
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coLogicalDevice* logicalDevice;
		coUint32 size8;
		coUint32 usage;
		coBool shared;
	};
	coRenderBuffer();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coLogicalDevice* logicalDevice;
	coUint32 size8;
};
