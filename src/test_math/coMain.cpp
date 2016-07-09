#include "test_math/pch.h"
#include "test/unit/coTestRegistry.h"

coInt main()
{
	coTestRegistry::instance->RunAllTests();

	return 0;
}
