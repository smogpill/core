// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coQuat
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coQuat() : x(0.f), y(0.f), z(0.f), w(1.0f) {}
	coFORCE_INLINE coQuat(coFloat _x, coFloat _y, coFloat _z, coFloat _w) : x(_x), y(_y), z(_z), w(_w) {}
	explicit coQuat(coNullPtr);

	coFloat x;
	coFloat y;
	coFloat z;
	coFloat w;
};
