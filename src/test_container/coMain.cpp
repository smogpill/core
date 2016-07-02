#include "test_container/pch.h"
#include "test/unit/coTestRegistry.h"

int main()
{
	coTestRegistry::instance->RunAllTests();

	return 0;
}
