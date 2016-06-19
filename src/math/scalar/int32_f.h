// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "lang/types/baseTypes.h"

namespace co
{
	const int32		int32_lastBit = sizeof(int32) * 8 - 1;
	const int32		int32_lastBitMask = (1 << int32_lastBit);

	coFORCE_INLINE int32 mod(int32 _a, int32 _b) { return _a % _b; }
	coFORCE_INLINE int32 sign(int32 _a) { return (_a > 0) - (_a < 0); }
	coFORCE_INLINE bool areSameSign(int32 _a, int32 _b) { return (_a ^ _b) >= 0; }
	coFORCE_INLINE bool areOppositeSign(int32 _a, int32 _b) { return (_a ^ _b) < 0; }
	coFORCE_INLINE int32 minWith0(int32 _a) { return (_a & (_a >> int32_lastBit)); }
	coFORCE_INLINE int32 maxWith0(int32 _a) { return (_a & ~(_a >> int32_lastBit)); }
	coFORCE_INLINE int32 pow2(int32 _a) { return _a << 1; }
	coFORCE_INLINE int32 pow3(int32 _a) { return (_a << 1) * _a; }
	coFORCE_INLINE int32 pow4(int32 _a) { return _a << 2; }
	coFORCE_INLINE int32 pow(int32 _a, uint32 _exponent)
	{
		int32 n = 1;
		while (_exponent--)
			n *= _a;
		return n;
	}

	// Eric Lengyel (Game engine Gems 2)
	coFORCE_INLINE int32 abs(int32 _a)
	{
		int32 m = _a >> int32_lastBit; // m = (x < 0) ? -1 : 0;
		return (_a ^ m) - m;
	}

	// Eric Lengyel (Game engine Gems 2)
	coFORCE_INLINE int32 nabs(int32 _a)
	{
		int32 m = _a >> int32_lastBit; // m = (x < 0) ? -1 : 0;
		return m - (_a ^ m);
	}

	// Eric Lengyel (Game engine Gems 2)
	template <int N>
	coFORCE_INLINE int32 incMod(int32 _a)
	{
		coASSERT(_a < N);
		return ((_a + 1) & ((_a - (N - 1)) >> int32_lastBit));
	}

	// Eric Lengyel (Game engine Gems 2)
	template <int N>
	coFORCE_INLINE int32 decMod(int32 _a)
	{
		coASSERT(_a < N);
		--_a;
		return (_a + ((_a >> int32_lastBit) & N));
	}

	// Eric Lengyel (Game engine Gems 2)
	coFORCE_INLINE int32 min(int32 _a, int32 _b)
	{
		const int32 c = _a - _b;
		return (_a - (c & ~(c >> int32_lastBit)));
	}

	// Eric Lengyel (Game engine Gems 2)
	coFORCE_INLINE int32 max(int32 _a, int32 _b)
	{
		const int32 c = _a - _b;
		return (_a - (c & (c >> int32_lastBit)));
	}

	/// http://graphics.stanford.edu/~seander/bithacks.html
	coFORCE_INLINE int32 negateIf(int32 _a, bool _negate)
	{
		return (_a ^ -static_cast<int32>(_negate)) + static_cast<int32>(_negate);
	}

	coFORCE_INLINE int32 clamp(int32 _a, int32 _min, int32 _max) { return min(_max, max(_min, _a)); }
	coFORCE_INLINE float exp2(int32 _a) { return bitCast<float>(clamp(_a + 127, 1, 254) << 23); }
}
