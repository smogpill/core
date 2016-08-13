// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "render/coRenderDeviceObject.h"

class coShader : public coRenderDeviceObject
{
	coDECLARE_SUPER(coRenderDeviceObject);
public:
	enum Stage
	{
		none,
		vertex,
		tesselationControl,
		tesselationEvaluation,
		geometry,
		fragment,
		compute
	};
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		const void* code;
		coUint32 codeSize8;
		Stage stage;
	};
	coShader();

	const Stage& GetStage() const { return stage; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	Stage stage;
};
