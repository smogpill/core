// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/scalar/coUint8_f.h"
#include "debug/log/coAssert.h"

const coUint32 coUint32_max = static_cast<coUint32>(-1);

coFORCE_INLINE coUint32 coMin(coUint32 _a, coUint32 _b) { return _a < _b ? _a : _b; }
coFORCE_INLINE coUint32 coMin(coUint32 _a, coUint32 _b, coUint32 _c) { return coMin(_a, coMin(_b, _c)); }
coFORCE_INLINE coUint32 coMax(coUint32 _a, coUint32 _b) { return _a > _b ? _a : _b; }
coFORCE_INLINE coUint32 coMax(coUint32 _a, coUint32 _b, coUint32 _c) { return coMax(_a, coMax(_b, _c)); }
coFORCE_INLINE coUint32 coMod(coUint32 _a, coUint32 _b) { return _a % _b; }
coFORCE_INLINE coUint32 coClamp(coUint32 _a, coUint32 _min, coUint32 _max) { return coMin(_max, coMax(_min, _a)); }
coFORCE_INLINE constexpr coUint32 coMod2(coUint32 _a) { return _a & 1; }
coFORCE_INLINE constexpr coUint32 coMod4(coUint32 _a) { return _a & 3; }
coFORCE_INLINE constexpr coUint32 coMod8(coUint32 _a) { return _a & 7; }
coFORCE_INLINE constexpr coUint32 coMod16(coUint32 _a) { return _a & 15; }
coFORCE_INLINE constexpr coUint32 coMod32(coUint32 _a) { return _a & 31; }
coFORCE_INLINE constexpr coUint32 coPow2(coUint32 _a) { return _a * _a; }
coFORCE_INLINE constexpr coUint32 coPow3(coUint32 _a) { return _a * _a * _a; }

/// 0 returns 0
coFORCE_INLINE coUint32 coHighestBitIndex(coUint32 _a)
{
#ifdef coMSVC_COMPILER
	unsigned long ret;
	_BitScanReverse(&ret, _a);
	return ret;
#else
	return _a ? (32 - __builtin_clz(_a)) : 0;
#endif
}

/// 0 returns 0
coFORCE_INLINE coUint32 coLowestBitIndex(coUint32 _a)
{
#ifdef coMSVC_COMPILER
	unsigned long ret;
	_BitScanForward(&ret, _a);
	return ret;
#else
	return _a ? (32 - __builtin_ctz(_a)) : 0;
#endif
}

constexpr coUint32 coLog2Immediate(coUint32 x) { return (x == 1) ? 0 : 1 + coLog2Immediate(x / 2); }
coFORCE_INLINE coUint32 coLog2(coUint32 x) { return coHighestBitIndex(x); }
coFORCE_INLINE coUint32 coPow4(coUint32 _a) { const coUint32 a2 = _a * _a; return a2 * a2; }
template <class F>
coFORCE_INLINE coUint32 coSetFlag(coUint32 _mask, const F& _flags, coBool _onoff) { return _onoff ? (_mask | static_cast<coUint32>(_flags)) : (_mask & ~static_cast<coUint32>(_flags)); }
template <class F>
coFORCE_INLINE constexpr coBool coAreAllRaised(coUint32 _mask, const F& _flags) { return (_mask & static_cast<coUint32>(_flags)) == static_cast<coUint32>(_flags); }
coFORCE_INLINE constexpr coUint32 coMakeMask(coUint32 _nbBits) { return (1 << _nbBits)-1; }

coFORCE_INLINE coUint32 coNextPowerOf2(coUint32 _a)
{
	--_a;
	_a |= _a >> 1;
	_a |= _a >> 2;
	_a |= _a >> 4;
	_a |= _a >> 8;
	_a |= _a >> 16;
	++_a;

	return _a;
}

coFORCE_INLINE constexpr coUint32 coNextMultipleOf(coUint32 a, coUint32 multiple)
{
	return a % multiple ? (a & ~multiple + multiple) : a;
}

coFORCE_INLINE coUint32 coPow(coUint32 _a, coUint32 _exponent)
{
	coUint32 n = 1;
	while (_exponent--)
		n *= _a;
	return n;
}

// http://graphics.stanford.edu/~seander/bithacks.html
coFORCE_INLINE coBool coIsPowerOf2(coUint32 _a)
{
	return (_a && !(_a & (_a - 1)));
}

// http://graphics.stanford.edu/~seander/bithacks.html
/// faster than isPowerOf2
coFORCE_INLINE coBool coIsPowerOf2Or0(coUint32 _a)
{
	return ((_a & (_a - 1)) == 0);
}

/// http://gurmeet.net/puzzles/fast-bit-counting-routines/
coFORCE_INLINE coUint32 coCountBits16(coUint32 _mask)
{
	coASSERT(_mask < 0x00010000u);
	return coUint8_popclut[_mask & 0xffu] + coUint8_popclut[_mask >> 8];
}

/// Check http://gurmeet.net/puzzles/fast-bit-counting-routines/
coFORCE_INLINE coUint32 coCountBits32(coUint32 _mask)
{
#ifdef coGCC
	return __builtin_popcount(_mask);
#else
	coUint32 result = coUint8_popclut[_mask & 0xffu];
	result += coUint8_popclut[(_mask >> 8) & 0xffu];
	result += coUint8_popclut[(_mask >> 16) & 0xffu];
	result += coUint8_popclut[_mask >> 24];

	return result;

	/* Seems to be slower than using a table

	/// MIT HAKMEM Count
	_a = _a - ((_a >> 1) & 0x55555555);
	_a = (_a & 0x33333333) + ((_a >> 2) & 0x33333333);
	return ((_a + (_a >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;*/
#endif
}

enum
{
	coRAND_MAX = 0x7fffffff,
};

coFORCE_INLINE coUint32 coNextRand(const coUint32 _previous)
{
	// Linear congruential generator
	// std::minstd_rand implementation
	return (48271 * _previous) % coRAND_MAX;
}

coFORCE_INLINE coUint32 coRand(coUint32& _seed)
{
	_seed = coNextRand(_seed);
	return _seed;
}

coFORCE_INLINE coUint32 coRand(coUint32& _seed, coUint32 _range)
{
	return coRand(_seed) % _range;
}

coFORCE_INLINE coUint32 coRand(coUint32& _seed, coUint32 from, coUint32 to)
{
	return from + (coRand(_seed) % (to - from));
}

coFORCE_INLINE coUint32 coRevert(coUint32 _a)
{
	_a = ((_a & 0xaaaaaaaa) >> 1) | ((_a & 0x55555555) << 1);
	_a = ((_a & 0xcccccccc) >> 2) | ((_a & 0x33333333) << 2);
	_a = ((_a & 0xf0f0f0f0) >> 4) | ((_a & 0x0f0f0f0f) << 4);
	_a = ((_a & 0xff00ff00) >> 8) | ((_a & 0x00ff00ff) << 8);
	return (_a >> 16) | (_a << 16);
}

coFORCE_INLINE coUint32 coRotl(coUint32 _a, coUint _shift)
{
#ifdef coMSVC_COMPILER
	return _rotl(_a, _shift);
#else
	return (_a << _shift) | (_a >> (32 - _shift));
#endif
}
