// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

namespace co
{
#define _coTEST(_testName_, ...) \
	static void _testName_(); \
	void _testName_()

#define coTEST(...) _coTEST(_test_##__COUNTER__, __VA_ARGS__)
}
