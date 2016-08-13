// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderBuffer.h"
#include "lang/result/coResult_f.h"

coRenderBuffer::coRenderBuffer()
	: size8(0)
{

}

coRenderBuffer::InitConfig::InitConfig()
	: size8(0)
	, shared(false)
	, usage(0)
{

}

coResult coRenderBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	size8 = config.size8;
	coTRY(size8 > 0, nullptr);
	return true;
}
