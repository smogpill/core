// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/scalar/uint8_f.h"
#include "debug/log.h"

namespace co
{
	const uint32 uint32_max = static_cast<uint32>(-1);

	coFORCE_INLINE uint32 min(uint32 _a, uint32 _b) { return _a < _b ? _a : _b; }
	coFORCE_INLINE uint32 min(uint32 _a, uint32 _b, uint32 _c) { return min(_a, min(_b, _c)); }
	coFORCE_INLINE uint32 max(uint32 _a, uint32 _b) { return _a > _b ? _a : _b; }
	coFORCE_INLINE uint32 max(uint32 _a, uint32 _b, uint32 _c) { return max(_a, max(_b, _c)); }
	coFORCE_INLINE uint32 mod(uint32 _a, uint32 _b) { return _a % _b; }
	coFORCE_INLINE uint32 mod2(uint32 _a) { return _a & 1; }
	coFORCE_INLINE uint32 mod4(uint32 _a) { return _a & 3; }
	coFORCE_INLINE uint32 mod8(uint32 _a) { return _a & 7; }
	coFORCE_INLINE uint32 mod16(uint32 _a) { return _a & 15; }
	coFORCE_INLINE uint32 mod32(uint32 _a) { return _a & 31; }
	coFORCE_INLINE uint32 pow2(uint32 _a) { return _a << 1; }
	coFORCE_INLINE uint32 pow3(uint32 _a) { return (_a << 1) * _a; }
	coFORCE_INLINE uint32 pow4(uint32 _a) { return _a << 2; }
	template <class F>
	coFORCE_INLINE uint32 setFlag(uint32 _mask, const F& _flags, bool _onoff) { return _onoff ? (_mask | static_cast<uint32>(_flags)) : (_mask & ~static_cast<uint32>(_flags)); }
	template <class F>
	coFORCE_INLINE bool areAllRaised(uint32 _mask, const F& _flags) { return (_mask & static_cast<uint32>(_flags)) == static_cast<uint32>(_flags); }

	coFORCE_INLINE uint32 nextPowerOf2(uint32 _a)
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

	coFORCE_INLINE uint32 pow(uint32 _a, uint32 _exponent)
	{
		uint32 n = 1;
		while (_exponent--)
			n *= _a;
		return n;
	}

	// http://graphics.stanford.edu/~seander/bithacks.html
	coFORCE_INLINE bool isPowerOf2(uint32 _a)
	{
		return (_a && !(_a & (_a - 1)));
	}

	// http://graphics.stanford.edu/~seander/bithacks.html
	/// faster than isPowerOf2
	coFORCE_INLINE bool isPowerOf2Or0(uint32 _a)
	{
		return ((_a & (_a - 1)) == 0);
	}

	/// http://gurmeet.net/puzzles/fast-bit-counting-routines/
	coFORCE_INLINE uint32 countBits16(uint32 _mask)
	{
		coASSERT(_mask < 0x00010000u);
		return uint8_popclut[_mask & 0xffu] + uint8_popclut[_mask >> 8];
	}

	/// Check http://gurmeet.net/puzzles/fast-bit-counting-routines/
	coFORCE_INLINE uint32 countBits32(uint32 _mask)
	{
	#ifdef coGCC
		return __builtin_popcount(_mask);
	#else
		uint32 result = uint8_popclut[_mask & 0xffu];
		result += uint8_popclut[(_mask >> 8) & 0xffu];
		result += uint8_popclut[(_mask >> 16) & 0xffu];
		result += uint8_popclut[_mask >> 24];

		return result;

		/* Seems to be slower than using a table

		/// MIT HAKMEM Count
		_a = _a - ((_a >> 1) & 0x55555555);
		_a = (_a & 0x33333333) + ((_a >> 2) & 0x33333333);
		return ((_a + (_a >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;*/
	#endif
	}

	/// From Intel.
	/// [opt] should be 2x faster than normal rand
	coFORCE_INLINE uint32 rand(uint32& _seed)
	{
		_seed = (214013 * _seed + 2531011);
		return (_seed >> 16) & 0x7FFF;
	}

	/// @param 0 returns 0
	coFORCE_INLINE uint32 highestBitIndex(uint32 _a)
	{
	#ifdef coMSVC_COMPILER
		unsigned long ret;
		_BitScanReverse(&ret, _a);
		return ret;
	#else
		return _a ? (32 - __builtin_clz(_a)) : 0;
	#endif
	}

	/// @param 0 returns 0
	coFORCE_INLINE uint32 lowestBitIndex(uint32 _a)
	{
	#ifdef coMSVC_COMPILER
		unsigned long ret;
		_BitScanForward(&ret, _a);
		return ret;
	#else
		return _a ? (32 - __builtin_ctz(_a)) : 0;
	#endif
	}

	coFORCE_INLINE uint32 reverse(uint32 _a)
	{
		_a = ((_a & 0xaaaaaaaa) >> 1) | ((_a & 0x55555555) << 1);
		_a = ((_a & 0xcccccccc) >> 2) | ((_a & 0x33333333) << 2);
		_a = ((_a & 0xf0f0f0f0) >> 4) | ((_a & 0x0f0f0f0f) << 4);
		_a = ((_a & 0xff00ff00) >> 8) | ((_a & 0x00ff00ff) << 8);
		return (_a >> 16) | (_a << 16);
	}

	coFORCE_INLINE uint32 rotl(uint32 _a, int _shift)
	{
	#ifdef coMSVC_COMPILER
		return _rotl(_a, _shift);
	#else
		return (_a << _shift) | (_a >> (32 - _shift));
	#endif
	}
}
