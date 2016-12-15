// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx3.h"

class alignas(16) coFloatx4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coFloatx4() {}
	coFORCE_INLINE coFloatx4(coFloat _xyzw) : x(_xyzw), y(_xyzw), z(_xyzw) {}
	coFORCE_INLINE coFloatx4(coFloat _x, coFloat _y, coFloat _z, coFloat _w) : x(_x), y(_y), z(_z), w(_w) {}
	coFORCE_INLINE coFloatx4(const coFloatx3& _xyz, coFloat _w) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w) {}

	coFORCE_INLINE operator const coFloatx3& () const { return reinterpret_cast<const coFloatx3&>(*this); }

	coFloat x;
	coFloat y;
	coFloat z;
	coFloat w;
};
