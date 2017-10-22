// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coRenderBuffer : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	enum Usage
	{
		vertex = 1<<0,
		index = 1<<1,
		uniform = 1<<2,
	};
	enum Type
	{
		default_,
		staging,
		dynamic
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coUint32 size8;
		coUint32 usage;
		Type type;
		coBool shared;
	};

	virtual coResult Map(void*& _data);
	virtual void Unmap();
	virtual coResult FlushMapped() { return true; }

	coUint32 GetSize8() const { return size8; }

protected:
	coRenderBuffer();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coUint32 size8;
};
