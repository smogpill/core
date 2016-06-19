// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/types/baseTypes.h"

namespace co
{
	struct alignas(16) int32x4
	{
		int32 x;
		int32 y;
		int32 z;
		int32 w;
	};
}
