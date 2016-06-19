// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/vector/floatx4.h"

namespace co
{
	struct alignas(16) quat
	{
		float x;
		float y;
		float z;
		float w;
	};
}
