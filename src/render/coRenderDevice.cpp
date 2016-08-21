// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderDevice.h"
#include "lang/result/coResult_f.h"

coRenderDevice::coRenderDevice()
	: context(nullptr)
{

}

coRenderDevice::InitConfig::InitConfig()
	: context(nullptr)
{

}

coResult coRenderDevice::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	context = config.context;
	coTRY(context, nullptr);
	return true;
}

coResult coRenderDevice::Submit(const SubmitConfig& /*_config*/)
{
	return true;
}

coResult coRenderDevice::WaitForIdle()
{
	return true;
}