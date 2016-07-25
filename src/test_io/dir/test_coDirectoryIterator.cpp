// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/dir/coDirectoryIterator.h"

coTEST(coDirectoryIterator, dot)
{
	coDirectoryIterator it;
	coDirectoryIterator::InitConfig config;
	config.path = ".";
	coEXPECT(it.Init(config));
}

coTEST(coDirectoryIterator, doubleDot)
{
	coDirectoryIterator it;
	coDirectoryIterator::InitConfig config;
	config.path = "..";
	coEXPECT(it.Init(config));
}
