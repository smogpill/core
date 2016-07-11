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
