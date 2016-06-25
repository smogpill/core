#include "test_math/pch.h"
#include "test/unit/coTestRegistry.h"

int main()
{
	coTestRegistry::instance->runAllTests();

	return 0;
}