// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coUint8_f.h"

coFORCE_INLINE coUint64 coMin(coUint64 _a, coUint64 _b) { return _a < _b ? _a : _b; }
coFORCE_INLINE coUint64 coMax(coUint64 _a, coUint64 _b) { return _a > _b ? _a : _b; }

coFORCE_INLINE coUint32 coCountBits(coUint64 _mask)
{
	coUint32 lo = static_cast<coUint32>(_mask);
	coUint32 hi = static_cast<coUint32>(_mask >> 32);
	coUint32 result = coUint8_popclut[lo & 0xffu] + coUint8_popclut[hi & 0xffu];
	result += coUint8_popclut[(lo >> 8) & 0xffu] + coUint8_popclut[(hi >> 8) & 0xffu];
	result += coUint8_popclut[(lo >> 16) & 0xffu] + coUint8_popclut[(hi >> 16) & 0xffu];
	result += coUint8_popclut[lo >> 24] + coUint8_popclut[hi >> 24];

	return result;
}

coFORCE_INLINE coUint64 coRotl(coUint64 _a, coUint _shift)
{
#ifdef coMSVC
	return _rotl64(_a, _shift);
#else
	return (_a << _shift) | (_a >> (64 - _shift));
#endif
}
