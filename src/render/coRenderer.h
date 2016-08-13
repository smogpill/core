// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coLogicalDevice;

class coRenderer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};

	coRenderer();
	virtual ~coRenderer();

	virtual coLogicalDevice* GetBestLogicalDeviceForSwapChain() const { return nullptr; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
};
