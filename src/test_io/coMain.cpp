#include "test_io/pch.h"
#include "test/unit/coTestRegistry.h"

coInt main()
{
	coTestRegistry::instance->RunAllTests();

	return 0;
}
