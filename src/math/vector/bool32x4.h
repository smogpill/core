// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/types/baseTypes.h"

namespace co
{
	struct alignas(16) Bool32x4
	{
		bool32 x;
		bool32 y;
		bool32 z;
		bool32 w;
	};
}
