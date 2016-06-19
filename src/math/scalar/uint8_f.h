// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"

namespace co
{
	extern const uint8 uint8_popclut[16 * 16];

	/// http://gurmeet.net/puzzles/fast-bit-counting-routines/
	coFORCE_INLINE uint8 countBits8(uint8 _mask)
	{
		return uint8_popclut[_mask];
	}

	// Sam Hocevar: http://lol.zoy.org/blog/2011/3/20/understanding-fast-float-integer-conversions
	coFORCE_INLINE float make_float(uint8 _a)
	{
		union { float f; uint32 i; } u; u.f = 32768.0f; u.i |= _a;
		return u.f - 32768.0f;
	}

	coFORCE_INLINE uint8 reverse(uint8 _a)
	{
		// return = (_n * 0x0202020202ULL & 0x010884422010ULL) % 1023; 64bits version
		return static_cast<uint8>(((_a * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
	}
}
