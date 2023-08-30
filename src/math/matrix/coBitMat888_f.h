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

coFORCE_INLINE coBitMat888 operator^(const coBitMat888& a, const coBitMat888& b)
{
	return coBitMat888(a._h[0] ^ b._h[0], a._h[1] ^ b._h[1]);
}

coFORCE_INLINE coBitMat888 operator&(const coBitMat888& a, const coBitMat888& b)
{
	return coBitMat888(a._h[0] & b._h[0], a._h[1] & b._h[1]);
}

coFORCE_INLINE coBitMat888 operator|(const coBitMat888& a, const coBitMat888& b)
{
	return coBitMat888(a._h[0] | b._h[0], a._h[1] | b._h[1]);
}

coFORCE_INLINE coBitMat888 operator~(const coBitMat888& m)
{
	return coBitMat888(~m._h[0], ~m._h[1]);
}

coFORCE_INLINE void coSetElement(coBitMat888& m, coUint x, coUint y, coUint z, coBool value)
{
	const coUint64 mask = 1ull << (y * 8 + x);
	if (value)
		m._h[z & 4][z & 3] |= mask;
	else
		m._h[z & 4][z & 3] &= ~mask;
}

template <class T>
coFORCE_INLINE void coVisitTrues(const coBitMat888& m, T func)
{
	// TODO: should be optimized

	for (coUint h = 0; h < 2; ++h)
	{
		const coUint64x4 hv = m._h[h];
		for (coUint hz = 0; hz < 4; ++hz)
		{
			const coUint z = h * 4 + hz;
			coUint64 zv = hv[hz];
			coUint index = 0;
			while (zv)
			{
				if (zv & 1)
					func(index % 8, index / 8, z);
				zv >>= 1;
				++index;
			}
		}
	}
}
