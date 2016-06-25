// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coConstString.h"

class _coTestFactory;

class _coTestInfo
{
public:
	_coTestInfo(const coConstString& _testCaseName, const coConstString& _testName, const coConstString& _sourcePath, coUint _sourceLine, _coTestFactory& _factory);
	const coConstString testCaseName;
	const coConstString testName;
	const coConstString sourcePath;
	const coUint sourceLine;
	_coTestFactory& factory;
};
