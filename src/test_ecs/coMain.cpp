// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_ecs/pch.h"
#include "test/unit/coTestRegistry.h"

coInt main()
{
	coTestRegistry::instance->RunAllTests();

	return 0;
}
