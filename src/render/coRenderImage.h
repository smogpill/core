// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"
#include "math/vector/coInt32x3.h"

class coRenderImage : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	enum Type
	{
		default,
		staging,
		dynamic
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coInt32x3 size;
		coUint32 arraySize;
		Type type;
	};
};
