// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAABox4.h"
#include "../../vector/coFloatx4_f.h"

coFORCE_INLINE coAABox4 coIntersect(const coAABox4& a, const coAABox4& b)
{
	return coAABox4(
		coMin(a._maxX, b._maxX),
		coMin(a._maxY, b._maxY),
		coMin(a._maxZ, b._maxZ),
		coMax(a._minX, b._minX),
		coMax(a._minY, b._minY),
		coMax(a._minZ, b._minZ)
	);
}

coFORCE_INLINE coBool32x4 coIsEmpty(const coAABox4& a)
{
	return a._minX > a._maxX | a._minY > a._maxY | a._minZ > a._maxZ;
}
