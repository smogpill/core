// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderDevice;
class coSwapChain;

class coRenderContext : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	
	virtual ~coRenderContext();

	virtual coResult Add(coSwapChain& _swapChain);

	virtual coRenderDevice* GetBestLogicalDeviceForSwapChain() const { return nullptr; }
	const coArray<coRenderDevice*> GetDevices() const { return devices; }

protected:
	coRenderContext();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coDynamicArray<coRenderDevice*> devices;
	coDynamicArray<coSwapChain*> swapChains;
};