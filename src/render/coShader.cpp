// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coShader.h"
#include "lang/result/coResult_f.h"

coShader::coShader()
	: logicalDevice(nullptr)
{

}

coShader::InitConfig::InitConfig()
	: logicalDevice(nullptr)
	, code(nullptr)
	, codeSize8(0)
{

}

coResult coShader::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	logicalDevice = config.logicalDevice;
	coTRY(config.code, nullptr);
	coTRY(config.codeSize8, nullptr);
	return true;
}
