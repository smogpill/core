// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileAccess.h"
#include "lang/result/coResult_f.h"

coFileAccess::coFileAccess()
	: mode(Mode::none)
	, impl(nullptr)
{

}

coFileAccess::~coFileAccess()
{
	OnImplShutdown();
}

coResult coFileAccess::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	path = config.path;
	SetDebugName(path);

	coTRY(OnImplInit(config), nullptr);

	return true;
}
