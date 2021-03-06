// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectoryIterator.h"
#include "lang/result/coResult_f.h"

coDirectoryIterator::coDirectoryIterator()
	: impl(GetDefaultImpl())
{
	OnImplConstruct();
}

coDirectoryIterator::~coDirectoryIterator()
{
	OnImplDestruct();
}

coDirectoryIterator::coDirectoryIterator(coDirectoryIterator&& _)
	: entry(std::move(_.entry))
	, impl(_.impl)
{
	_.impl = GetDefaultImpl();
}

coResult coDirectoryIterator::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	const InitConfig& config = static_cast<const InitConfig&>(_config);
	coTRY(OnImplInit(config), "Implementation failed to init");
	return true;
}
