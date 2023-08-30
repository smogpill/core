// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coBitMat888.h"
#include "../vector/coUint64x4_f.h"

coFORCE_INLINE coBool coBitMat888::operator()(coUint x, coUint y, coUint z) const
{
	coASSERT(z < 8);
	const coUint64x4 half = this->_h[z & 4];
	const coUint64 xy = half[z & 3];
	return (xy >> (y * 8 + x)) != 0;
}

coFORCE_INLINE void coBitMat888::operator= (coBool b)
{
	_h[0] = _h[1] = coUint64(-coInt64(b));
}

coFORCE_INLINE void coSetElement(coBitMat888& m, coUint x, coUint y, coUint z, coBool value)
{
	const coUint64 mask = 1ull << (y * 8 + x);
	if (value)
		m._h[z & 4][z & 3] |= mask;
	else
		m._h[z & 4][z & 3] &= ~mask;
}
