// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coShader.h"
#include "lang/result/coResult_f.h"

coShader::coShader()
	: stage(Stage::none)
{

}

coShader::InitConfig::InitConfig()
	: code(nullptr)
	, codeSize8(0)
	, stage(Stage::none)
{

}

coResult coShader::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	stage = config.stage;
	coTRY(config.code, nullptr);
	coTRY(config.codeSize8, nullptr);
	coTRY(stage != Stage::none, nullptr);
	return true;
}
