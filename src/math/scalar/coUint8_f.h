// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"

extern const coUint8 coUint8_popclut[16 * 16];

/// http://gurmeet.net/puzzles/fast-bit-counting-routines/
coFORCE_INLINE coUint8 coCountBits8(coUint8 _mask)
{
	return coUint8_popclut[_mask];
}

// Sam Hocevar: http://lol.zoy.org/blog/2011/3/20/understanding-fast-float-integer-conversions
coFORCE_INLINE coFloat coMake_float(coUint8 _a)
{
	return coBitCast<coFloat>(static_cast<coUint32>(_a) | coBitCast<coUint32>(32768.0f)) - 32768.0f;
}

coFORCE_INLINE coUint8 coReverse(coUint8 _a)
{
	// return = (_n * 0x0202020202ULL & 0x010884422010ULL) % 1023; 64bits version
	return static_cast<coUint8>(((_a * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
}
