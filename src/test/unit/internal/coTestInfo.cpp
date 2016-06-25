// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "test/pch.h"
#include "test/unit/internal/coTestInfo.h"

_coTestInfo::_coTestInfo(const coConstString& _testCaseName, const coConstString& _testName, const coConstString& _sourcePath, coUint _sourceLine, _coTestFactory& _factory)
	: testCaseName(_testCaseName)
	, testName(_testName)
	, sourcePath(_sourcePath)
	, sourceLine(_sourceLine)
	, factory(_factory)
{

}
