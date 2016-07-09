// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "lang/result/coResult_f.h"

coResult coAppImpl::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	//const InitConfig& config = static_cast<const InitConfig&>(_config);

	return true;
}
