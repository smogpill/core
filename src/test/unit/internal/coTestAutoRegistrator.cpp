// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "test/pch.h"
#include "test/unit/internal/coTestAutoRegistrator.h"
#include "test/unit/coTestRegistry.h"

_coTestAutoRegistrator::_coTestAutoRegistrator(const _coTestInfo& _info)
{
	coTestRegistry::CreateInstanceIfMissing();
	coTestRegistry::instance->Add(_info);
}
