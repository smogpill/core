// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectoryAccess.h"
#include "lang/result/coResult_f.h"

coResult coDirectoryAccess::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig config = static_cast<const InitConfig&>(_config);
	path = config.path;
	return true;
}

coDirectoryIterator coDirectoryAccess::Begin()
{
	coDirectoryIterator it;
	coDirectoryIterator::InitConfig config;
	config.path = path;
	if (it.Init(config))
	{
		return it;
	}
	else
	{
		return End();
	}
}

const coDirectoryIterator& coDirectoryAccess::End()
{
	static const coDirectoryIterator it;
	return it;
}
