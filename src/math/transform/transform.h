// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/quaternion/quat.h"
#include "math/vector/floatx4.h"

namespace co
{
	struct transform
	{
		quat rotation;
		floatx4 translation;
		floatx4 scale;
	};
}
