// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderDeviceObject.h"
#include "lang/result/coResult_f.h"

coRenderDeviceObject::coRenderDeviceObject()
	: device(nullptr)
{

}

coRenderDeviceObject::InitConfig::InitConfig()
	: device(nullptr)
{

}

coResult coRenderDeviceObject::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	device = config.device;
	coTRY(device, nullptr);

	return true;
}
