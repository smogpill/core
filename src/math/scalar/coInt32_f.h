// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"

const coInt32 coInt32_lastBit = sizeof(coInt32) * 8 - 1;
const coInt32 coInt32_lastBitMask = (1 << coInt32_lastBit);

coFORCE_INLINE coInt32 coMod(coInt32 _a, coInt32 _b) { return _a % _b; }
coFORCE_INLINE coInt32 sign(coInt32 _a) { return (_a > 0) - (_a < 0); }
coFORCE_INLINE coBool coAreSameSign(coInt32 _a, coInt32 _b) { return (_a ^ _b) >= 0; }
coFORCE_INLINE coBool coAreOppositeSign(coInt32 _a, coInt32 _b) { return (_a ^ _b) < 0; }
coFORCE_INLINE coInt32 coMinWith0(coInt32 _a) { return (_a & (_a >> coInt32_lastBit)); }
coFORCE_INLINE coInt32 coMaxWith0(coInt32 _a) { return (_a & ~(_a >> coInt32_lastBit)); }
coFORCE_INLINE coInt32 coPow2(coInt32 _a) { return _a << 1; }
coFORCE_INLINE coInt32 coPow3(coInt32 _a) { return (_a << 1) * _a; }
coFORCE_INLINE coInt32 coPow4(coInt32 _a) { return _a << 2; }
coFORCE_INLINE coInt32 coPow(coInt32 _a, coUint32 _exponent)
{
	coInt32 n = 1;
	while (_exponent--)
		n *= _a;
	return n;
}

// Eric Lengyel (Game engine Gems 2)
coFORCE_INLINE coInt32 coAbs(coInt32 _a)
{
	coInt32 m = _a >> coInt32_lastBit; // m = (x < 0) ? -1 : 0;
	return (_a ^ m) - m;
}

// Eric Lengyel (Game engine Gems 2)
coFORCE_INLINE coInt32 coNabs(coInt32 _a)
{
	coInt32 m = _a >> coInt32_lastBit; // m = (x < 0) ? -1 : 0;
	return m - (_a ^ m);
}

// Eric Lengyel (Game engine Gems 2)
template <int N>
coFORCE_INLINE coInt32 coIncMod(coInt32 _a)
{
	coASSERT(_a < N);
	return ((_a + 1) & ((_a - (N - 1)) >> coInt32_lastBit));
}

// Eric Lengyel (Game engine Gems 2)
template <coInt32 N>
coFORCE_INLINE coInt32 coDecMod(coInt32 _a)
{
	coASSERT(_a < N);
	--_a;
	return (_a + ((_a >> coInt32_lastBit) & N));
}

// Eric Lengyel (Game engine Gems 2)
coFORCE_INLINE coInt32 coMin(coInt32 _a, coInt32 _b)
{
	const coInt32 c = _a - _b;
	return (_a - (c & ~(c >> coInt32_lastBit)));
}

// Eric Lengyel (Game engine Gems 2)
coFORCE_INLINE coInt32 coMax(coInt32 _a, coInt32 _b)
{
	const coInt32 c = _a - _b;
	return (_a - (c & (c >> coInt32_lastBit)));
}

/// http://graphics.stanford.edu/~seander/bithacks.html
coFORCE_INLINE coInt32 coNegateIf(coInt32 _a, coBool _negate)
{
	return (_a ^ -static_cast<coInt32>(_negate)) + static_cast<coInt32>(_negate);
}

coFORCE_INLINE coInt32 coClamp(coInt32 _a, coInt32 _min, coInt32 _max) { return coMin(_max, coMax(_min, _a)); }
coFORCE_INLINE coFloat coExp2(coInt32 _a) { return coBitCast<coFloat>(coClamp(_a + 127, 1, 254) << 23); }
