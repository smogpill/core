// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coBitMat888.h"
#include "coUint64x4.h"

coFORCE_INLINE coBool coBitMat888::operator()(coUint x, coUint y, coUint z) const
{
	coASSERT(z < 8);
	const coUint64x4 half = m.x[z & 4];
	const coUint64 xy = half[z & 3];
	return (xy >> (y * 8 + x)) != 0;
}

void coBitMat888::operator= (coBool b)
{
	x[0] = x[1] = coUint64(-b);
}
