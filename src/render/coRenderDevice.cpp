// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderDevice.h"

coResult coRenderDevice::Submit(const SubmitConfig& /*_config*/)
{
	return true;
}

coResult coRenderDevice::WaitForIdle()
{
	return true;
}