#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/dir/coDirectoryAccess.h"

coTEST(coDirectoryAccess, dotIterate)
{
	coDirectoryAccess a;
	coDirectoryAccess::InitConfig config;
	config.path = ".";
	coEXPECT(a.Init(config));
	coUint nb = 0;
	for (const coDirectoryEntry& entry : a)
	{
		if (entry.name.count > 0)
			++nb;
	}
	coEXPECT(nb > 0);
}
