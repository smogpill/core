// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coRenderImage;

class coRenderImageView : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	enum Type
	{
		default,
		tex1D,
		tex2D,
		tex3D,
		texCube
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coRenderImage* image;
		Type type;
	};

	coRenderImageView();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coRenderImage* image;
};
