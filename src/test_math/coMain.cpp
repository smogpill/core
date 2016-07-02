#include "test_math/pch.h"
#include "test/unit/coTestRegistry.h"

int main()
{
	coTestRegistry::instance->RunAllTests();

	return 0;
}
