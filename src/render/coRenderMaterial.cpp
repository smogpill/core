// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderMaterial.h"
#include "lang/result/coResult_f.h"

coRenderMaterial::coRenderMaterial()
	: vertexShader(nullptr)
	, fragmentShader(nullptr)
{

}

coRenderMaterial::InitConfig::InitConfig()
	: vertexShader(nullptr)
	, fragmentShader(nullptr)
{

}

coResult coRenderMaterial::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	vertexShader = config.vertexShader;
	fragmentShader = config.fragmentShader;
	return true;
}
